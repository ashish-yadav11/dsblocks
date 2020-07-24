#include <string.h>

#define ICON			COL1 "" COL0

#include "../util.h"
#include "hotspot.h"

void
hotspotu(char *str, int sigval)
{
        if (sigval == 1)
                strcpy(str, ICON);
        else if (sigval == 0)
                *str = '\0';
}

void
hotspotc(int button)
{
        cspawn((char *[]){ "/usr/bin/sudo", "/home/ashish/.scripts/hotspot.sh", "terminate", NULL });
}
