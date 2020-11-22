#!/bin/dash
hotspot=/home/ashish/.scripts/hotspot.sh
notify="notify-send -h string:x-canonical-private-synchronous:hotspot"

if [ "$(create_ap --list-running | wc -l)" -gt 2 ] ; then
    sudo $hotspot terminate
else
    if nmcli -t -f DEVICE,STATE device status | grep -qE 'wlp5s0:(connected)|(connecting)' ; then
        $notify -t 3000 Hotspot "Your adapter can not be a station and an AP at the same time"
        exit
    fi
    blockinfo=$(rfkill -nr -o SOFT,TYPE,HARD | grep -m1 ' wlan ')
    if [ "${blockinfo##* }" = blocked ] ; then
        $notify -t 1000 Hotspot "Wifi is soft blocked"
        exit
    elif [ "${blockinfo%% *}" = blocked ] ; then
        $notify -t 1000 Hotspot "Wifi is hard blocked"
        exit
    fi
    sudo $hotspot initiate ||
        $notify -u critical Hotspot "Some error occured in initiating hotspot"
fi
