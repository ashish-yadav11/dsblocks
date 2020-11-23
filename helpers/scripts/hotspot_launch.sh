#!/bin/dash
hotspot=/home/ashish/.scripts/hotspot.sh
notify="notify-send -h string:x-canonical-private-synchronous:hotspot"

if [ -n "$(create_ap --list-running)" ] ; then
    sudo $hotspot terminate
else
    if nmcli -t -f DEVICE,STATE device status | grep -qE 'wlp5s0:(connected)|(connecting)' ; then
        $notify -t 3000 Hotspot "Your adapter can not be a station and an AP at the same time"
        exit
    fi
    case $(rfkill -nro TYPE,SOFT,HARD) in
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
