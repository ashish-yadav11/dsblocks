#include <stdio.h>

#include "../util.h"
#include "volume.h"

#define ICON0				COL3 "" COL0
#define ICON1				COL3 "" COL0

#define BUFLENGTH			10

#define PAMIXER				(char *[]){ "/usr/bin/pamixer", "--get-mute", "--get-volume", NULL }

#define PAVUCONTROL			(char *[]){ "/usr/bin/pavucontrol-qt", NULL }
#define SETDEFAULTVOL			(char *[]){ "/usr/bin/pamixer", "--set-volume", "50", NULL }
#define TOGGLEMUTE			(char *[]){ "/usr/bin/pamixer", "--toggle-mute", NULL }

void
volumeu(char *str, int sigval)
{
        char buf[BUFLENGTH];

        buf[getcmdout(PAMIXER, buf, BUFLENGTH) - 1] = '\0';
        if (buf[0] == 'f') /* output was `false <volume>' */
                snprintf(str, CMDLENGTH, ICON0 "%s%%", buf + 6);
        else /* output was `true <volume>' */
                snprintf(str, CMDLENGTH, ICON1 "%s%%", buf + 5);
}

void
volumec(int button)
{
        switch(button) {
                case 1:
                        cspawn(TOGGLEMUTE);
                        break;
                case 3:
                        cspawn(PAVUCONTROL);
                        break;
                case 2:
                        cspawn(SETDEFAULTVOL);
                        break;
        }
}
