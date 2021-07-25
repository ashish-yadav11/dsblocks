#!/bin/sh
# This script should be placed in /usr/lib/systemd/system-sleep/
# It updates blocks when waking up from sleep
case "$1" in
    post)
        sigdsblocks 1 # calendar
        sigdsblocks 2 # volume
        sigdsblocks 4 # battery
        sigdsblocks 6 # cpu temperature
        ;;
esac
