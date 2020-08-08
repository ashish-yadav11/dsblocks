#!/bin/dash
exec >/dev/null 2>&1

# allow only two instances of the script at one time
if read -r lock </tmp/mailsync.lock ; then
    case "$lock" in
        0) echo 1 >/tmp/mailsync.lock ;;
        1) exit ;;
    esac
else
    echo 0 >/tmp/mailsync.lock
    remove=1
fi

# main body
sigdsblocks 2 1
if ping -c1 imap.gmail.com ; then
    PID=$(pidof -s /usr/bin/mbsync) && tail --pid="$PID" -f /dev/null
    sigdsblocks 2 -2
    if mbsync iiser ; then
        sigdsblocks 2 3
    else
        sigdsblocks 2 4
    fi
    notmuch new
else
    sigdsblocks 2 5
fi

# cleanup
if [ -n "$remove" ] ; then
    rm -f /tmp/mailsync.lock
else
    echo 0 >/tmp/mailsync.lock
fi
