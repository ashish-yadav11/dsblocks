#!/bin/sh
# This script should be placed in /etc/NetworkManager/dispatcher.d/
# NetworkManager-dispatcher.service should be active
# It signals mail block to sync and update
case "$2" in
    up|down) sigdsblocks 3 ;;
esac
