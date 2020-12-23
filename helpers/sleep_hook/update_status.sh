#!/bin/sh

# This script is only for systemd-logind users
# It should be placed in /usr/lib/systemd/system-sleep/

case $1 in
    post)
        sigdsblocks 1 # calendar
        sigdsblocks 2 # volume
        sigdsblocks 4 # battery
        sigdsblocks 6 # cpu temperature
        ;;
esac
