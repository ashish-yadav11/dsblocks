#!/bin/dash
pactl subscribe |
    while read -r output ; do
        case $output in
            *"sink "*) sigdsblocks 2 ;;
        esac
    done
