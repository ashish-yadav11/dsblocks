#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ICON			COL1 "" COL0

#include "../util.h"
#include "hotspot.h"

void
hotspotu(char *str, int sigval)
{
        if (sigval)
                strcpy(str, ICON);
        else
                *str = '\0';
}

void
hotspotc(int button)
{
//      TODO: turn off hotspot
}
