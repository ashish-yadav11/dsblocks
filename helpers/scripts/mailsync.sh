#!/bin/sh
# This script syncs and indexes mail database using mbsync and notmuch and
# signals mail block whether the sync was successful or not
mbsync_channel=-a

# At max two instances of the script are allowed to run simultaneously, one
# syncing and one waiting for the other to finish syncing.

sigdsblocks 3 -1
exec >/dev/null 2>&1 8<>/tmp/mailsync.1.lock 9<>/tmp/mailsync.2.lock
if ! flock -n 8 ; then
    flock -n 9 || exit
    if ! flock -w120 8 ; then
        read -r PID <&8 && pkill mbsync -P "$PID"
        flock -w15 8 || exit
    fi
fi
echo "$$" >&8
mbsync "$mbsync_channel"
: >/tmp/mailsync.1.lock
sigval=$?
notmuch new
flock -n 9 && sigdsblocks 3 "$sigval"
