#!/bin/sh
# (for pipewire users)
# This script sets volume of both sides of output to the greatest factor of
# "step" less than the smaller of the two
# It is executed by volume block on middle click
sink=$(pactl info | awk '$1 == "Default" && $2 == "Sink:" {print $3}')
[ -n "$sink" ] || exit
volume=$(
    pactl list sinks | awk -v sink="$sink" '
        BEGIN {
            step = 5
        }
        f {
            if ($1 == "Volume:") {
                v = $3 <= $10 ? $5 : $12
                sub(/%/, "", v)
                v = int(v / step) * step
                exit
            }
            next
        }
        $1 == "Name:" && $2 == sink {
            f = 1
        }
        END {
            if (f)
                print v
            else
                exit 1
        }
    '
) && pactl set-sink-volume @DEFAULT_SINK@ "$volume%"
