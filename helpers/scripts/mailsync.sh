#!/bin/dash
exec >/dev/null 2>&1

exec 8>/tmp/mailsync.1.lock
# Check if some other instance of the script is already
# running which hasn't crossed the pinging stage
flock -n 8 || { sigdsblocks 2 1; exit ;}

exec 9>/tmp/mailsync.2.lock
# Check if some other instance of the script is already
# running which has crossed the pinging state
if flock -n 9 ; then
    sigdsblock 2 1
else
    sigdsblocks 2 2
    # Wait for the other script to finish
    flock 9 || exit
    sigdsblock 2 -1
fi

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
    sigdsblocks 2 -4
fi
