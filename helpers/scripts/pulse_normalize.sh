#!/bin/sh
# (for pulseaudio users)
# This script sets volume of both sides of output to the greatest factor of
# "step" less than the smaller of the two
# It is executed by volume block on middle click
volume="$(
    pacmd list-sinks | awk '
        BEGIN {
            step = 5
        }
        f {
            if ($1 == "volume:") {
                v = $3 <= $10 ? $5 : $12
                sub(/%/, "", v)
                v = int(v / step + 1/2) * step
                exit
            }
        }
        $1 == "*" && $2 == "index:" {
            f = 1
        }
        END {
            if (f)
                print v
            else
                exit 1
        }
    '
)" && pactl set-sink-volume @DEFAULT_SINK@ "$volume%"
