#include <string.h>

#include "../util.h"
#include "hotspot.h"

#define ICON                    COL1 ""

#define TURNOFFHOTSPOT          (char *[]){ "/usr/bin/sudo", SCRIPT("hotspot.sh"), "terminate", NULL }

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
