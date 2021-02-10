#!/bin/sh
# This is a helper script for volume block
# It prints info about current volume, mute and whether headhone is plugged or
# not in a manner parsable by volume block
pacmd list-sinks | awk '
    {
        if (f) {
            if ($1 == "index:") {
                exit
            }
            if ($1 == "muted:" && $2 == "yes") {
                i += 1
            } else if ($1 == "volume:") {
                if ($3 == $10) {
                    vb = $5
                } else {
                    vl = $5
                    vr = $12
                }
            } else if ($1 == "active" && $2 == "port:" && $3 ~ /headphone/) {
                i += 2
            }
        } else if ($1 == "*" && $2 == "index:") {
            f = 1
        }
    }
    END {
        if (f) {
            if (vb) {
                printf "%d%s", i, vb
            } else {
                printf "%dL%s R%s", i, vl, vr
            }
        }
    }
'
