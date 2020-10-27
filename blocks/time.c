#include <time.h>

#include "../util.h"
#include "time.h"

#define ICON                    COL1 "" COL0

void
timeu(char *str, int sigval)
{
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        strftime(str, CMDLENGTH, ICON "%R", &tm);
}
