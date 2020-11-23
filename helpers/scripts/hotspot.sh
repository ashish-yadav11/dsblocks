#!/bin/dash
case $1 in
    initiate)
        iwi=$(nmcli -t -f DEVICE connection show --active | head -n1)
        trap 'sigdsblocks 4 0; exit' HUP INT TERM
        sigdsblocks 4 1
        create_ap wlp5s0 "${iwi:-eno1}" hp 98761234; status=$?
        sigdsblocks 4 0
        exit "$status"
        ;;
    terminate)
        create_ap --stop wlp5s0
        ;;
    fix-unmanaged)
        [ -z "$(create_ap --list-running)" ] && create_ap --fix-unmanaged
        ;;
    *)
        echo "Usage: $0 initiate|terminate|fix-unmanaged"
    ;;
esac
