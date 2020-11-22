#!/bin/dash

# This script is only for NetworkManager users
# It should be placed in /etc/NetworkManager/dispatcher.d/

case $2 in
    up|down) /home/ashish/.local/bin/sigdsblocks 3 ;;
esac
