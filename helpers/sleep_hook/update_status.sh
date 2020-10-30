#!/bin/dash
case $1 in
    post)
        /home/ashish/.local/bin/sigdsblocks 10 # time
        /home/ashish/.local/bin/sigdsblocks 5  # calendar
        /home/ashish/.local/bin/sigdsblocks 4  # battery
        /home/ashish/.local/bin/sigdsblocks 6  # cpu temperature
        ;;
esac
