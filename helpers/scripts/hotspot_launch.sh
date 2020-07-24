#!/bin/dash
if [ "$(create_ap --list-running | wc -l)" -gt 2 ] ; then
    sudo /home/ashish/.scripts/hotspot.sh terminate
    exit
else
    nmcli -t -f DEVICE,STATE device status | grep -qE 'wlp5s0:(connected)|(connecting)' &&
        { dunstify -r 81520 -t 3000 "Hotspot" "Your adapter can not be a station and an AP at the same time"; exit ;}
    blockinfo=$(rfkill -nr -o SOFT,TYPE,HARD | grep -m1 " wlan ")
    if [ "${blockinfo##* }" = blocked ] ; then
        dunstify -r 81520 -t 1000 "Hotspot" "Wifi is soft blocked"
        exit
    elif [ "${blockinfo%% *}" = blocked ] ; then
        dunstify -r 81520 -t 1000 "Hotspot" "Wifi is hard blocked"
        exit
    fi
    sudo /home/ashish/.scripts/hotspot.sh initiate ||
        dunstify -r 81520 -u critical "Hotspot" "Some error occured in initiating hotspot"
fi
