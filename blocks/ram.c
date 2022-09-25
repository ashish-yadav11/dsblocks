#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "ram.h"

#define ICONn                           COL1 "" COL0
#define ICONw                           COL2 "" COL0

#define MEMW                            85
#define SWPW                            85

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
        size_t val, memtotal = 0, swptotal = 0;
        size_t memu = 0, swpu = 0;

        if (!(fp = fopen(RAMFILE, "r"))) {
                *str = '\0';
                return 1;
        }
        do {
                if (fscanf(fp, "%s %zu kB", field, &val) != 2) {
                        fclose(fp);
                        *str = '\0';
                        return 1;
                }
                if (strncmp(field, MEMTOTAL, sizeof MEMTOTAL) == 0) {
                        memtotal = val;
                } else if (strncmp(field, MEMAVAIL, sizeof MEMAVAIL) == 0) {
                        memu = 100 - (val * 100) / memtotal;
                } else if (strncmp(field, SWPTOTAL, sizeof SWPTOTAL) == 0) {
                        swptotal = val;
                } else if (strncmp(field, SWPAVAIL, sizeof SWPAVAIL) == 0) {
                        swpu = 100 - (val * 100) / swptotal;
                }
        } while (strncmp(field, LASTFIELDTOSCAN, sizeof LASTFIELDTOSCAN) != 0);
        fclose(fp);

        if (memtotal != 0 && swptotal != 0) {
                if (memu >= MEMW && swpu >= SWPW)
                        return SPRINTF(str, ICONw "s%zu%% r%zu%%", swpu, memu);
                return SPRINTF(str, ICONn "s%zu%% r%zu%%", swpu, memu);
        } else if (memtotal != 0) {
                if (memu >= MEMW)
                        return SPRINTF(str, ICONw "%zu%%", memu);
                return SPRINTF(str, ICONn "%zu%%", memu);
        } else {
                *str = '\0';
                return 1;
        }
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
