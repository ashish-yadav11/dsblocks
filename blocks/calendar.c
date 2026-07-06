#include <time.h>

#include "../util.h"
#include "calendar.h"

#define ICON                    COL1 "" COL0

#define TOGGLECALCURSE          (char *[]){ SCRIPT("sigdwm"), "scrt i 4", NULL }

size_t
calendaru(char *str, int sigval)
{
        static int mode = 0;
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        if (sigval == 0)
                mode = (mode + 1) % 3;

        switch (mode) {
                case 1:
                        return strftime(str, BLOCKLENGTH, ICON "%a, %b %d, %T", &tm) + 1;
                case 2:
                        return strftime(str, BLOCKLENGTH, ICON "%a, %b %d", &tm) + 1;
                default:
                        return strftime(str, BLOCKLENGTH, ICON "%a, %b %d, %R", &tm) + 1;
        }
}

void
calendarc(int button)
{
        switch (button) {
                case 1:
                        csigself(1, 0);
                        break;
                case 3:
                        cspawn(TOGGLECALCURSE);
                        break;
        }
}
