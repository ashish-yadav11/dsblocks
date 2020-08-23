#!/bin/dash
exec >/dev/null 2>&1

lockfile1=/tmp/mailsync.1.lock
lockfile2=/tmp/mailsync.2.lock

exec 8>"$lockfile1" 9>"$lockfile2"

if ! flock -n 8 ; then
    flock -n 9 || exit
#    trap 'rm -rf "$lockfile2"; exit' EXIT HUP INT TERM
    flock -w 300 8 || exit
#    trap 'rm -rf "$lockfile1" "$lockfile2"; exit' EXIT HUP INT TERM
#else
#    trap 'rm -rf "$lockfile1"; exit' EXIT HUP INT TERM
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
