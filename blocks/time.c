#include <stdio.h>
#include <time.h>

#include "../util.h"
#include "time.h"

#define ICON                    COL1 "" COL0

void
timeu(char *str, int sigval)
{
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        snprintf(str, CMDLENGTH, ICON "%02d:%02d", tm.tm_hour, tm.tm_min);
}
