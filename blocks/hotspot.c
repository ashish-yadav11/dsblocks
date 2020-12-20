#include <string.h>

#include "../util.h"
#include "hotspot.h"

#define ICON                            COL1 ""

#define TURNOFFHOTSPOT                  (char *[]){ "/usr/bin/sudo", SCRIPT("hotspot.sh"), "terminate", NULL }

void
hotspotu(char *str, int enabled)
{
        if (enabled)
                strcpy(str, ICON);
        else
                *str = '\0';
}

void
hotspotc(int button)
{
        cspawn(TURNOFFHOTSPOT);
}
