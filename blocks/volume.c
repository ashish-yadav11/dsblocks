#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

#define BUFLENGTH			10

void
volumeu(char *str, int *sigval)
{
        char buf[BUFLENGTH];
        char *arg[] = { "/usr/bin/pamixer", "--get-mute", "--get-volume", NULL };

        buf[getcmdout(arg, buf, BUFLENGTH) - 1] = '\0';
        if (buf[0] == 'f') /* output was `false x..%` */
                snprintf(str, CMDLENGTH, VOL0 "%s%%", buf + 6);
        else /* output was `true x..%` */
                snprintf(str, CMDLENGTH, VOL1 "%s%%", buf + 5);
}

void
volumec(int button)
{
        switch(button) {
                case 1:
                {       char *arg[] = { "/usr/bin/pamixer", "--toggle-mute", NULL };

                        execv(arg[0], arg);
                        goto error;
                }
                case 2:
                {       char *arg[] = { "/usr/bin/pavucontrol-qt", NULL };

                        execv(arg[0], arg);
                        goto error;
                }
                case 3:
                {       char *arg[] = { "/usr/bin/pamixer", "--set-volume", "60", NULL };

                        execv(arg[0], arg);
                        goto error;
                }
        }
error:
        perror("volumec - execv");
        _exit(127);
}
