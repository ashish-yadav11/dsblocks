#!/bin/dash
# This script should be autostarted on startup
# It signals volume block to update on playerctl status change
lockfile="$XDG_RUNTIME_DIR/playerdaemon.lock"

exec 9<>"$lockfile"
if ! flock -n 9 ; then
    echo "playerdaemon: another instance already active, exiting!" >&2
    exit 2
fi

playerctl status --follow |
    while IFS='' read -r state ; do
        case "$state" in
            Paused)  sigdsblocks 2 -1 ;;
            Playing) sigdsblocks 2  1 ;;
        esac
    done
