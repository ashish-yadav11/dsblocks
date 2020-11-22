#include <stdio.h>

#include "../util.h"
#include "volume.h"

#define ICONh0                          COL1 "" COL0
#define ICONh1                          COL2 "" COL0
#define ICONs0                          COL1 "" COL0
#define ICONs1                          COL2 "" COL0

#define BUFLENGTH                       10

#define HEADPHONESTATUS                 (char *[]){ SCRIPT("headphone_status.sh"), NULL }
#define PAMIXER                         (char *[]){ "/usr/bin/pamixer", "--get-mute", "--get-volume", NULL }

#define PAVUCONTROL                     (char *[]){ "/usr/bin/pavucontrol-qt", NULL }
#define SETDEFAULTVOL                   (char *[]){ "/usr/bin/pamixer", "--set-volume", "50", NULL }
#define TOGGLEMUTE                      (char *[]){ "/usr/bin/pamixer", "--toggle-mute", NULL }

static int headphone;

static void
updateheadphone()
{
        char buf[1];

        getcmdout(HEADPHONESTATUS, buf, 1);
        headphone = buf[0] == '1' ? 1 : 0;
}

void
volumeu(char *str, int sigval)
{
        char buf[BUFLENGTH];

        if (sigval < 0 || sigval == NILL)
                updateheadphone();
        buf[getcmdout(PAMIXER, buf, BUFLENGTH) - 1] = '\0';
        if (buf[0] == 'f') /* output was `false <volume>' */
                snprintf(str, CMDLENGTH, "%s%s%%", headphone ? ICONh0 : ICONs0, buf + 6);
        else /* output was `true <volume>' */
                snprintf(str, CMDLENGTH, "%s%s%%", headphone ? ICONh1 : ICONs1, buf + 5);
}

void
volumec(int button)
{
        switch(button) {
                case 1:
                        cspawn(TOGGLEMUTE);
                        break;
                case 2:
                        cspawn(SETDEFAULTVOL);
                        break;
                case 3:
                        cspawn(PAVUCONTROL);
                        break;
        }
}
