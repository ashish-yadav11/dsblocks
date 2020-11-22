#!/bin/dash
snd_card_num=1

if grep -A3 'Headphone Playback Switch' "/proc/asound/card$snd_card_num/codec#0" |
       grep -qF 'Amp-Out vals:  [0x00 0x00]' ; then
    echo -n 1
else
    echo -n 0
fi
