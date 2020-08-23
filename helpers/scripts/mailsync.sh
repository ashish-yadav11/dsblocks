#!/bin/dash
exec >/dev/null 2>&1 8>/tmp/mailsync.1.lock

if ! flock -n 8 ; then
    exec 9>/tmp/mailsync.2.lock
    flock -n 9 || exit
    flock -w 300 8 || exit
fi

sigdsblocks 2 1
if ping -c1 imap.gmail.com ; then
    sigdsblocks 2 -2
    if mbsync iiser ; then
        sigdsblocks 2 3
    else
        sigdsblocks 2 4
    fi
    notmuch new
else
    sigdsblocks 2 4
fi
