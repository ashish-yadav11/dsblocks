#include <stdio.h>

#include "../util.h"
#include "volume.h"

#define ICONsn                          COL1 "" COL0
#define ICONsm                          COL2 "" COL0
#define ICONhn                          COL1 "" COL0
#define ICONhm                          COL2 "" COL0

#define PULSEINFO                       (char *[]){ SCRIPT("pulse_info.sh"), NULL }

#define PAVUCONTROL                     (char *[]){ "pavucontrol-qt", NULL }
#define NORMALIZEVOLUME                 (char *[]){ SCRIPT("pulse_normalize.sh"), NULL }
#define TOGGLEMUTE                      (char *[]){ "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL }

size_t
volumeu(char *str, int sigval)
{
        static char *icons[] = { ICONsn, ICONsm, ICONhn, ICONhm };
        char buf[32];
        size_t l;

        if (!(l = getcmdout(PULSEINFO, buf, sizeof buf - 1))) {
                *str = '\0';
                return 1;
        }
        buf[l] = '\0';
        return SPRINTF(str, "%s%s", icons[buf[0] - '0'], buf + 1);
}

void
volumec(int button)
{
        switch(button) {
                case 1:
                        cspawn(TOGGLEMUTE);
                        break;
                case 2:
                        cspawn(NORMALIZEVOLUME);
                        break;
                case 3:
                        cspawn(PAVUCONTROL);
                        break;
        }
}
