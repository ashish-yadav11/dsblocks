#include <stdio.h>

#include "../util.h"
#include "cputemp.h"

#define ICONn                           COL1 "" COL0
#define ICONc                           COL2 "" COL0

#define WARNCPUTEMP                     70000

#define CPUTEMPFILE                     "/sys/class/thermal/thermal_zone0/temp"

size_t
cputempu(char *str, int sigval)
{
        int temp;

        if (!readint(CPUTEMPFILE, &temp)) {
                *str = '\0';
                return 1;
        }
        if (temp < WARNCPUTEMP)
                return SPRINTF(str, ICONn "%d°C", temp / 1000);
        else
                return SPRINTF(str, ICONc "%d°C", temp / 1000);
}

void
cputempc(int button)
{
        switch (button) {
                case 1:
                        TERMCMD("htop", "-s", "PERCENT_CPU");
                        break;
                case 2:
                        TERMCMD("htop");
                        break;
                case 3:
                        TERMCMD("htop", "-s", "PERCENT_MEM");
                        break;
        }
}
