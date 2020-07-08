#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

#define WARNCPUTEMP			70000
#define	CPUTEMPFILE			"/sys/class/thermal/thermal_zone0/temp"

void
cputempu(char *str, int *sigval)
{
        int temp;
        FILE *fp;

        if (!(fp = fopen(CPUTEMPFILE, "r"))) {
                *str = '\0';
                perror("cputempu - fopen");
                return;
        }
        if (fscanf(fp, "%d", &temp) != 1) {
                *str = '\0';
                perror("cputempu - fscanf");
                return;
        }
        fclose(fp); 

        if (temp < WARNCPUTEMP)
                snprintf(str, CMDLENGTH, CTMP0 "%d°C", (temp + 999) / 1000);
        else
                snprintf(str, CMDLENGTH, CTMP1 "%d°C", (temp + 999) / 1000);
}

void
cputempc(int button)
{
        switch(button) {
                case 1:
                {       char *arg[] = { "/usr/bin/termite", "-e", "htop -s PERCENT_CPU", NULL };

                        execv(arg[0], arg);
                        perror("cputempc - execv");
                        _exit(127);
                }
                case 2:
                {       char *arg[] = { "/usr/bin/termite", "-e", "htop", NULL };

                        execv(arg[0], arg);
                        perror("cputempc - execv");
                        _exit(127);
                }
                case 3:
                {       char *arg[] = { "/usr/bin/termite", "-e", "htop -s PERCENT_MEM", NULL };

                        execv(arg[0], arg);
                        perror("cputempc - execv");
                        _exit(127);
                }
        }
}
