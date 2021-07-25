#!/bin/sh
# This script is a wrapper on hotspot.sh to create hotspot
# This is supposed to be bound to a key combination
# Change the path to hotspot.sh below
hotspot=/home/ashish/.scripts/hotspot.sh
notify="notify-send -h string:x-canonical-private-synchronous:hotspot"

if [ -n "$(create_ap --list-running)" ] ; then
    sudo $hotspot terminate
else
    case "$(rfkill -nro TYPE,SOFT,HARD)" in
        *"wlan blocked"*)
            $notify -t 1000 Hotspot "Wifi is soft blocked"
            exit
            ;;
        *"wlan unblocked blocked"*)
            $notify -t 1000 Hotspot "Wifi is hard blocked"
            exit
            ;;
    esac
    sudo $hotspot initiate ||
        $notify -u critical Hotspot "Some error occured in initiating hotspot"
fi
