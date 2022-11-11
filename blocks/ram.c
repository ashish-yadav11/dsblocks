#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "ram.h"

#define ICONn                           COL1 "" COL0
#define ICONw                           COL2 "" COL0

#define MEMW                            90

#define RAMFILE                         "/proc/meminfo"

#define MEMTOTAL                        "MemTotal:"
#define MEMAVAIL                        "MemAvailable:"
#define SWPTOTAL                        "SwapTotal:"
#define SWPAVAIL                        "SwapFree:"
#define LASTFIELDTOSCAN                 "Dirty:"

size_t
ramu(char *str, int sigval)
{
        FILE *fp;
        char field[32];
        uintmax_t val, memavail = 0, memtotal = 0, swpavail = 0, swptotal = 0;
        unsigned int memu, swpu;

        if (!(fp = fopen(RAMFILE, "r"))) {
                *str = '\0';
                return 1;
        }
        do {
                if (fscanf(fp, "%s %ju kB", field, &val) != 2) {
                        fclose(fp);
                        *str = '\0';
                        return 1;
                }
                if (!memtotal && strncmp(field, MEMTOTAL, sizeof MEMTOTAL) == 0) {
                        memtotal = val;
                        continue;
                } else if (!memavail && strncmp(field, MEMAVAIL, sizeof MEMAVAIL) == 0) {
                        memavail = val;
                        continue;
                } else if (!swptotal && strncmp(field, SWPTOTAL, sizeof SWPTOTAL) == 0) {
                        swptotal = val;
                        continue;
                } else if (!swpavail && strncmp(field, SWPAVAIL, sizeof SWPAVAIL) == 0) {
                        swpavail = val;
                        break;
                }
        } while (strncmp(field, LASTFIELDTOSCAN, sizeof LASTFIELDTOSCAN) != 0);
        fclose(fp);

        if (!memtotal) {
                *str = '\0';
                return 1;
        }
        memu = 100 - (memavail * 100) / memtotal;
        if (!swptotal) {
                if (memu >= MEMW)
                        return SPRINTF(str, ICONw "%u%%", memu);
                else
                        return SPRINTF(str, ICONn "%u%%", memu);
        }
        swpu = 100 - (swpavail * 100) / swptotal;
        if (memu + swpu >= 2 * MEMW)
                return SPRINTF(str, ICONw "s%u%% r%u%%", swpu, memu);
        else
                return SPRINTF(str, ICONn "s%u%% r%u%%", swpu, memu);
}

void
ramc(int button)
{
        switch (button) {
                case 1:
                        TERMCMD("htop", "-s", "PERCENT_MEM");
                        break;
                case 2:
                        TERMCMD("htop");
                        break;
                case 3:
                        TERMCMD("htop", "-s", "PERCENT_CPU");
                        break;
        }
}
