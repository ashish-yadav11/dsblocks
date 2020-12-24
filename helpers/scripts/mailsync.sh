#!/bin/sh
mbsync_channel=-a

exec >/dev/null 2>&1

# At max two instances of the script are allowed to run simultaneously,
# one syncing and one waiting for the other to finish syncing.
exec 8>/tmp/mailsync.1.lock
if ! flock -n 8 ; then
    exec 9>/tmp/mailsync.2.lock
    flock -n 9 || exit
    flock 8
fi

sigdsblocks 3 0

if mbsync "$mbsync_channel" ; then
    notmuch new
    sigdsblocks 3 1
else
    notmuch new
    sigdsblocks 3 2
fi
