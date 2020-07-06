#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

#define BATCAPFILE			"/sys/class/power_supply/BAT0/capacity"
#define	ACSTATFILE			"/sys/class/power_supply/AC/online"

void
batteryu(char *str, int *sigval)
{
        int bat, ac;
        FILE *fp;

        if (!(fp = fopen(ACSTATFILE, "r"))) {
                *str = '\0';
                perror("batteryu - fopen");
                return;
        }
        if (fscanf(fp, "%d", &ac) != 1) {
                *str = '\0';
                perror("batteryu - fscanf");
                return;
        }
        fclose(fp);
        if (!(fp = fopen(BATCAPFILE, "r"))) {
                // on ac power
        }
        if (fscanf(fp, "%d", &bat) != 1) {
                *str = '\0';
                perror("batteryu - fscanf");
                return;
        }
        fclose(fp); 

        snprintf(str, CMDLENGTH, BAT0 "%d%%", bat);
}

void
batteryc(int button)
{
        switch (button) {
                case 1:
                {
                }
                        break;
                case 2:
                {
                }
                        break;
                case 3:
                {
                }
                        break;
        }
}
