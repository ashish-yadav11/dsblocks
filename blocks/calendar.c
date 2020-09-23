#include <stdio.h>
#include <time.h>

#include "../util.h"
#include "calendar.h"

#define ICON                    COL1 "" COL0

#define SHOWCALCURSE            (char *[]){ "/usr/bin/xsetroot", "-name", "z:tscr i 4", NULL }

void
calendaru(char *str, int sigval)
{
        static char *days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        static char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                                  "Aug", "Sep", "Oct", "Nov", "Dec" };
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        snprintf(str, CMDLENGTH, ICON "%s, %02d %s",
                 days[tm.tm_wday], tm.tm_mday, months[tm.tm_mon]);
}

void
calendarc(int button)
{
        cspawn(SHOWCALCURSE);
}
