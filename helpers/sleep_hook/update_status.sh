#!/bin/dash

# This script is only for systemd-logind users
# It should be placed in /usr/lib/systemd/system-sleep/

case $1 in
    post)
        sigdsblocks=/home/ashish/.local/bin/sigdsblocks

        $sigdsblocks 1 # calendar
        $sigdsblocks 2 # volume
        $sigdsblocks 4 # battery
        $sigdsblocks 6 # cpu temperature
        ;;
esac
