#include <time.h>

#include "../util.h"
#include "calendar.h"

#define ICON                    COL1 "" COL0

#define SHOWCALCURSE            (char *[]){ "/usr/bin/xsetroot", "-name", "z:tscr i 4", NULL }

void
calendaru(char *str, int sigval)
{
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        strftime(str, BLOCKLENGTH, ICON "%a, %b %d, %R", &tm);
}

void
calendarc(int button)
{
        cspawn(SHOWCALCURSE);
}
