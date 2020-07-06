#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

void
hotspotu(char *str, int *sigval)
{
        if (*sigval)
                strcpy(str, HTSPT);
        else
                *str = '\0';
}

void
hotspotc(int button)
{
//      TODO: turn off hotspot
}
