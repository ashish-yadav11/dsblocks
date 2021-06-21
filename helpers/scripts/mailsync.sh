#!/bin/sh
# This script syncs and indexes mail database using mbsync and notmuch and
# signals mail block whether the sync was successful or not
mbsync_channel=-a

lockfile1=$XDG_RUNTIME_DIR/mailsync.1.lock
lockfile2=$XDG_RUNTIME_DIR/mailsync.2.lock

# At max two instances of the script are allowed to run simultaneously, one
# syncing and one waiting for the other to finish syncing.

sigdsblocks 3 -1
exec >/dev/null 2>&1 8<>"$lockfile1" 9<>"$lockfile2"
if ! flock -n 8 ; then
    flock -n 9 || exit
    flock 8
fi
mbsync "$mbsync_channel"
sigval=$?
notmuch new
flock -n 9 && sigdsblocks 3 "$sigval"
