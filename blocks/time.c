#include <stdio.h>
#include <time.h>

#include "../util.h"
#include "time.h"

void
timeu(char *str, int sigval)
{
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        snprintf(str, CMDLENGTH, CLOCK "%02d:%02d", tm.tm_hour, tm.tm_min);
}
