#!/bin/sh
# This script should be autostarted on startup
# It signals volume block to update on audio related events
pactl subscribe |
    while IFS='' read -r event ; do
        case "$event" in
            *" sink "*) sigdsblocks 2 ;;
        esac
    done
