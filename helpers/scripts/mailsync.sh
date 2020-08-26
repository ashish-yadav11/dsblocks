#!/bin/dash
exec >/dev/null 2>&1

sigdsblocks 2 1

exec 8>/tmp/mailsync.1.lock
# Exit if some other instance of the script is
# running which hasn't crossed the pinging stage
flock -n 8 || exit

exec 9>/tmp/mailsync.2.lock
# Wait while some other instance of the script is
# running which has crossed the pinging state
flock 9 || exit

if ping -c1 imap.gmail.com ; then
    exec 8>/dev/null
    sigdsblocks 2 -2
    if mbsync iiser ; then
        sigdsblocks 2 3
    else
        sigdsblocks 2 4
    fi
    notmuch new
else
    exec 8>/dev/null
    sigdsblocks 2 -5
fi
