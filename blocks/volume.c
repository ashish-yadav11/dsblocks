#include <stdio.h>

#include "../util.h"
#include "volume.h"

#define ICON0				COL1 "" COL0
#define ICON1				COL1 "" COL0

#define BUFLENGTH			10

void
volumeu(char *str, int sigval)
{
        char buf[BUFLENGTH];
        char *arg[] = { "/usr/bin/pamixer", "--get-mute", "--get-volume", NULL };

        buf[getcmdout(arg, buf, BUFLENGTH) - 1] = '\0';
        if (buf[0] == 'f') /* output was `false x..%` */
                snprintf(str, CMDLENGTH, ICON0 "%s%%", buf + 6);
        else /* output was `true x..%` */
                snprintf(str, CMDLENGTH, ICON1 "%s%%", buf + 5);
}

void
volumec(int button)
{
        switch(button) {
                case 1:
                        cspawn((char *[]){ "/usr/bin/pamixer", "--toggle-mute", NULL });
                        break;
                case 3:
                        cspawn((char *[]){ "/usr/bin/pamixer", "--set-volume", "50", NULL });
                        break;
                case 2:
                        cspawn((char *[]){ "/usr/bin/pavucontrol-qt", NULL });
                        break;
        }
}
