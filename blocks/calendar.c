#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

void
calendaru(char *str, int *sigval)
{
        char *day;
        char *month;
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        switch (tm.tm_wday) {
                case 0:  day   = "Sun"; break;
                case 1:  day   = "Mon"; break;
                case 2:  day   = "Tue"; break;
                case 3:  day   = "Wed"; break;
                case 4:  day   = "Thu"; break;
                case 5:  day   = "Fri"; break;
                default: day   = "Sat"; break;
        }
        switch (tm.tm_mon) {
                case 0:  month = "Jan"; break;
                case 1:  month = "Feb"; break;
                case 2:  month = "Mar"; break;
                case 3:  month = "Apr"; break;
                case 4:  month = "May"; break;
                case 5:  month = "Jun"; break;
                case 6:  month = "Jul"; break;
                case 7:  month = "Aug"; break;
                case 8:  month = "Sep"; break;
                case 9:  month = "Oct"; break;
                case 10: month = "Nov"; break;
                default: month = "Dec"; break;
        }
        snprintf(str, CMDLENGTH, CAL "%s, %02d %s", day, tm.tm_mday, month);
}

void
calendarc(int button)
{
        switch (button) {
                case 1:
                {
                }
                        break;
                case 2:
                {
                }
                        break;
                case 3:
                {
                }
                        break;
        }
}
