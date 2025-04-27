#!/usr/bin/python
# This script or <player_daemon.sh> should be autostarted on startup
# It signals volume block to update on audio playback resume/pause events

# Additionally, it also implement MPRIS functionality:
# run: `dbus-send --print-reply=literal --dest=org.python.mpriscustomdaemon /org/python/mpriscustomdaemon org.python.mpriscustomdaemon.<command> string:'<parameter>'
#   command can be: doaction with parameter: play|pause|toggle|next|previous
#               or  query    with parameter: status


import dbus
import dbus.service
import dbus.mainloop.glib
from gi.repository import GLib
import subprocess
import signal


DBUS_DAEMON = 'org.freedesktop.DBus'
DBUS_PATH = '/org/freedesktop/DBus'
DBUS_IFACE = 'org.freedesktop.DBus'
PROPS_IFACE = 'org.freedesktop.DBus.Properties'
MPRIS_PREFIX = 'org.mpris.MediaPlayer2.'
PLAYER_IFACE = 'org.mpris.MediaPlayer2.Player'
PLAYER_PATH  = '/org/mpris/MediaPlayer2'

SERVICE_NAME = 'org.python.mpriscustomdaemon'
OBJECT_PATH = '/org/python/mpriscustomdaemon'


active_players = []
paused_players = []


signal.signal(signal.SIGCHLD, signal.SIG_IGN)

def sigvolblock(active):
    subprocess.Popen(["sigdsblocks", "2", str(active)])


def hook_player(bus, player_name):
    try:
        proxy = bus.get_object(player_name, PLAYER_PATH)
        iface = dbus.Interface(proxy, PROPS_IFACE)
        allprops = iface.GetAll(PLAYER_IFACE)
        status = allprops.get('PlaybackStatus', '<no PlaybackStatus>')
        if status == "Playing":

            if not active_players:
                sigvolblock(1) # signal dsblocks, playback turned on
            active_players.append(player_name)

        elif status == "Paused":
            paused_players.append(player_name)
#       print(f"{player_name} initial status: {status}")

        def on_properties_changed(iface, changed_props, invalidated_props):
            if 'PlaybackStatus' in changed_props:
                if changed_props['PlaybackStatus'] == "Playing":
                    paused_players[:] = (p for p in paused_players if p != player_name)

                    if not active_players:
                        sigvolblock(1) # signal dsblocks, playback turned on
                    active_players.append(player_name)

                elif changed_props['PlaybackStatus'] == "Paused":

                    if active_players: # signal dsblocks, playback paused
                        active_players[:] = (p for p in active_players if p != player_name)
                        if not active_players:
                            sigvolblock(-1)

                    paused_players.append(player_name)
                else:

                    if active_players:
                        active_players[:] = (p for p in active_players if p != player_name)
                        if not active_players:
                            sigvolblock(-1)

                    paused_players[:] = (p for p in paused_players if p != player_name)
#               print(f"{player_name} -> {changed_props['PlaybackStatus']}")

        bus.add_signal_receiver(
            handler_function=on_properties_changed,
            signal_name='PropertiesChanged',
            dbus_interface=PROPS_IFACE,
            bus_name=player_name,
            path=PLAYER_PATH
        )
    except dbus.DBusException as e:
#       print(f"{player_name} vanished before hookup: {e.get_dbus_message()}")
        return

def on_name_owner_changed(player_name, old_owner, new_owner):
    if not player_name.startswith(MPRIS_PREFIX):
        return
    if old_owner == '' and new_owner != '':
#       print(f"{player_name} appeared")
        hook_player(bus, player_name)
    elif old_owner != '' and new_owner == '':

        if active_players:
            active_players[:] = (p for p in active_players if p != player_name)
            if not active_players:
                sigvolblock(-1)

        paused_players[:] = (p for p in paused_players if p != player_name)
#       print(f"{player_name} disappeared")


def playresume(player_name):
    try:
        proxy = bus.get_object(player_name, PLAYER_PATH)
        dbus.Interface(proxy, PLAYER_IFACE).Play()
    except:
        pass

def playpause(player_name):
    try:
        proxy = bus.get_object(player_name, PLAYER_PATH)
        dbus.Interface(proxy, PLAYER_IFACE).Pause()
    except:
        pass

def playnext(player_name):
    try:
        proxy = bus.get_object(player_name, PLAYER_PATH)
        dbus.Interface(proxy, PLAYER_IFACE).Next()
    except:
        pass

def playprev(player_name):
    try:
        proxy = bus.get_object(player_name, PLAYER_PATH)
        dbus.Interface(proxy, PLAYER_IFACE).Previous()
    except:
        pass


def mprisplay():
    if active_players or not paused_players:
        return
    playresume(paused_players[-1])

def mprispause():
    for name in active_players:
        playpause(name)

def mpristoggle():
    if active_players:
        mprispause()
    elif paused_players:
        playresume(paused_players[-1])

def mprisnext(n):
    if active_players:
        name = active_players[-1]
    elif paused_players:
        name = paused_players[-1]
    else:
        return
    if (n >= 0):
        playnext(name)
    else:
        playprev(name)


class mpriscontrol(dbus.service.Object):
    def __init__(self, bus):
        dbus.service.Object.__init__(self, bus, OBJECT_PATH)

    """
    @dbus.service.method(SERVICE_NAME, in_signature='', out_signature='s')
    def ping(self):
        print("Received Ping from outside!")
        return "Pong!"
    """

    @dbus.service.method(SERVICE_NAME, in_signature='s', out_signature='')
    def doaction(self, action):
        if action == "play":
            mprisplay()
        elif action == "pause":
            mprispause()
        elif action == "toggle":
            mpristoggle()
        elif action == "next":
            mprisnext(1)
        elif action == "previous":
            mprisnext(-1)

    @dbus.service.method(SERVICE_NAME, in_signature='s', out_signature='s')
    def query(self, data):
        if data == "status":
            if active_players:
                return f"{active_players[-1]}: active"
            elif paused_players:
                return f"{paused_players[-1]}: paused"
        return ""


if __name__ == '__main__':
    # Set up the GLib mainloop integration
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus = dbus.SessionBus()

    # Publish our service
    servicename = dbus.service.BusName(SERVICE_NAME, bus)
    control = mpriscontrol(bus)

    # 1) Bootstrap existing players
    for name in bus.list_names():
        if name.startswith(MPRIS_PREFIX):
            hook_player(bus, name)

    # 2) Listen for future players appearing/disappearing
    bus.add_signal_receiver(
        handler_function=on_name_owner_changed,
        signal_name='NameOwnerChanged',
        dbus_interface=DBUS_IFACE,
        path=DBUS_PATH
    )

    # 3) Run forever
    GLib.MainLoop().run()
