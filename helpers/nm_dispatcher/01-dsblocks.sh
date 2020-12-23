#!/bin/sh

# This script is only for NetworkManager users
# It should be placed in /etc/NetworkManager/dispatcher.d/

case $2 in
    up|down) sigdsblocks 3 ;;
esac
