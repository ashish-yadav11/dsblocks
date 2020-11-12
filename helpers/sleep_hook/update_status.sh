#!/bin/dash
case $1 in
    post)
        /home/ashish/.local/bin/sigdsblocks 1 # calendar
        /home/ashish/.local/bin/sigdsblocks 4 # battery
        /home/ashish/.local/bin/sigdsblocks 6 # cpu temperature
        ;;
esac
