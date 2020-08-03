#include <stdio.h>
#include <time.h>

#include "../util.h"
#include "calendar.h"

#define ICON			COL2 "" COL0

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
        switch (button) {
                case 1:
                        break;
                case 3:
                        break;
                case 2:
                        break;
        }
}
