#!/bin/dash
exec >/dev/null 2>&1

read -r lock </tmp/mailsync.lock
[ -n "$lock" ] && exit

sigdsblocks 2 1
if ping -c1 imap.gmail.com ; then
    if PID=$(pidof -s /usr/bin/mbsync) ; then
        echo 1 >/tmp/mailsync.lock
        remove=1
        tail --pid="$PID" -f /dev/null
    fi
    sigdsblocks 2 -2
    [ -n "$remove" ] && rm -f /tmp/mailsync.lock
    if mbsync iiser ; then
        sigdsblocks 2 3
    else
        sigdsblocks 2 4
    fi
    notmuch new
else
    sigdsblocks 2 5
fi
