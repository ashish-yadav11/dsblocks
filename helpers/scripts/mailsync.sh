#!/bin/dash
exec >/dev/null 2>&1
sigdsblocks 2 1
if ping -c1 imap.gmail.com ; then
    sigdsblocks 2 -2
    while PID=$(pidof -s /usr/bin/mbsync) ; do
        tail --pid="$PID" -f /dev/null
    done
    if mbsync iiser ; then
        sigdsblocks 2 3
    else
        sigdsblocks 2 4
    fi
    notmuch new
else
    sigdsblocks 5
fi
