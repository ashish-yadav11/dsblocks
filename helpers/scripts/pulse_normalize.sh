#!/bin/sh
# sets volume to the greatest factor of "step" less than the current value
volume=$(
    pacmd list-sinks | awk '
        BEGIN {
            step = 5
        }
        {
            if (f) {
                if ($1 == "index:") {
                    exit
                }
                if ($1 == "volume:") {
                    v = $3 <= $10 ? $5 : $12
                    gsub(/%/, "", v)
                    v = int(v / step) * step
                }
            } else if ($1 == "*" && $2 == "index:") {
                f = 1
            }
        }
        END {
            if (f) {
                print v
            } else {
                exit 1
            }
        }
    '
) && pactl set-sink-volume @DEFAULT_SINK@ "$volume%"
