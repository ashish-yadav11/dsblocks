#!/bin/sh
# (for pipewire users)
# This is a helper script for volume block
# It prints info about current volume, mute and whether headhone is plugged or
# not in a manner parsable by volume block
sink="$(pactl get-default-sink)"
[ -n "$sink" ] || exit
pactl list sinks | awk -v sink="$sink" '
    f {
        if ($1 == "Mute:" && $2 == "yes") {
            i += 1
        } else if ($1 == "Volume:") {
            if ($3 == $10) {
                vb = $5
            } else {
                vl = $5
                vr = $12
            }
        } else if ($1 == "Active" && $2 == "Port:") {
            if (tolower($3) ~ /headphone/)
                i += 2
            exit
        }
        next
    }
    $1 == "Name:" && $2 == sink {
        f = 1
    }
    END {
        if (f) {
            if (vb)
                printf "%d%s", i, vb
            else
                printf "%dL%s R%s", i, vl, vr
        }
    }
'
