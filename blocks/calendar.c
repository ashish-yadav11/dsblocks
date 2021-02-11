#include <time.h>

#include "../util.h"
#include "calendar.h"

#define ICON                    COL1 "" COL0

#define TOGGLECALCURSE          (char *[]){ SCRIPT("sigdwm"), "scrt i 4", NULL }

size_t
calendaru(char *str, int sigval)
{
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        return strftime(str, BLOCKLENGTH, ICON "%a, %b %d, %R", &tm) + 1;
}

void
calendarc(int button)
{
        cspawn(TOGGLECALCURSE);
}
