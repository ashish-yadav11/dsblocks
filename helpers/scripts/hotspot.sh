#!/bin/sh
# This script is a wrapper on create_ap
# It is supposed that sudo is setup such that running this script with sudo
# doesn't ask for password
# The script signals hotspot block to update on initiating and terminating
# hotspot
wifi_interface=wlp5s0

case "$1" in
    initiate)
        fallback_interface=eno1

        nmcli device disconnect "$wifi_interface"
        active_interface="$(nmcli -t -f DEVICE connection show --active)"
        newline='
'
        active_interface="${active_interface%%"$newline"*}"
        trap 'sigdsblocks 4 0; exit 0' HUP INT TERM
        sigdsblocks 4 1
        create_ap "$wifi_interface" "${active_interface:-"$fallback_interface"}" pc 11111111
        status="$?"
        sigdsblocks 4 0
        exit "$status"
        ;;
    terminate)
        create_ap --stop "$wifi_interface"
        ;;
    fix-unmanaged)
        [ -z "$(create_ap --list-running)" ] && create_ap --fix-unmanaged
        ;;
    *)
        echo "Usage: $0 initiate|terminate|fix-unmanaged"
        ;;
esac
