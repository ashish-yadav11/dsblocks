#!/bin/dash
exec >/dev/null 2>&1

# to make sure no more than 2 instances can be active at the same time
if read -r numactive </tmp/mailsync.num ; then
    case "$numactive" in
        0)
            echo 1 >/tmp/mailsync.num
            remove=1
            ;;
        1)
            echo 2 >/tmp/mailsync.num
            remove=0
            ;;
        *)
            exit ;;
    esac
else
    echo 1 >/tmp/mailsync.num
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
case "$remove" in
    0)
        echo 1 >/tmp/mailsync.num
        ;;
    1)
        rm -f /tmp/mailsync.num
        ;;
esac
