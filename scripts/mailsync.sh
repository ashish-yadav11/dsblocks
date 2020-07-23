#!/bin/dash
exec >/dev/null 2>&1
sigdsblocks 2 1
if ping -c1 imap.gmail.com ; then
    sigdsblocks 2 -2
    PID=$(pidof -s /usr/bin/mbsync)
    [ -n "$PID" ] && tail --pid="$PID" -f /dev/null
    if mbsync iiser ; then
        sigdsblocks 2 3
    else
        sigdsblocks 2 4
    notmuch new
else
    sigdsblocks 5
fi
