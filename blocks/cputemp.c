#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../util.h"
#include "cputemp.h"

#define WARNCPUTEMP			70000
#define	CPUTEMPFILE			"/sys/class/thermal/thermal_zone0/temp"

#define TERMCMD(cmd) \
	cspawn((char *[]) { "/usr/bin/termite", "-e", cmd, NULL })

void
cputempu(char *str, int sigval)
{
        int temp;

        if (!readint(CPUTEMPFILE, &temp)) {
                *str = '\0';
                return;
        }
        if (temp < WARNCPUTEMP)
                snprintf(str, CMDLENGTH, CTMP0 "%d°C", (temp + 999) / 1000);
        else
                snprintf(str, CMDLENGTH, CTMP1 "%d°C", (temp + 999) / 1000);
}

void
cputempc(int button)
{
        switch (button) {
                case 1:
                        TERMCMD("htop -s PERCENT_CPU");
                        break;
                case 2:
                        TERMCMD("htop");
                        break;
                case 3:
                        TERMCMD("htop -s PERCENT_MEM");
                        break;
        }
}
