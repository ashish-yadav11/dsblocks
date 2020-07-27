#include <string.h>

#include "../util.h"
#include "hotspot.h"

#define ICON			COL1 "" COL0

#define TURNOFFHOTSPOT		(char *[]){ "/usr/bin/sudo", "/home/ashish/.scripts/hotspot.sh", "terminate", NULL }

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
        cspawn(TURNOFFHOTSPOT);
}
