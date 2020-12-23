#include <stdio.h>

#include "../util.h"
#include "cputemp.h"

#define ICONn                           COL1 "" COL0
#define ICONh                           COL2 "" COL0

#define WARNCPUTEMP                     70000

#define CPUTEMPFILE                     "/sys/class/thermal/thermal_zone0/temp"

void
cputempu(char *str, int sigval)
{
        int temp;

        if (!readint(CPUTEMPFILE, &temp)) {
                *str = '\0';
                return;
        }
        if (temp < WARNCPUTEMP)
                snprintf(str, BLOCKLENGTH, ICONn "%d°C", temp / 1000);
        else
                snprintf(str, BLOCKLENGTH, ICONh "%d°C", temp / 1000);
}

void
cputempc(int button)
{
        switch (button) {
                case 1:
                        TERMCMD("/usr/bin/htop -s PERCENT_CPU");
                        break;
                case 2:
                        TERMCMD("/usr/bin/htop");
                        break;
                case 3:
                        TERMCMD("/usr/bin/htop -s PERCENT_MEM");
                        break;
        }
}
