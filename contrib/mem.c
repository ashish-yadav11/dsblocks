#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "mem.h"

#define ICONmem                           COL1 "\uf538" COL0

static const char meminfofile[] = "/proc/meminfo";

size_t
memu(char *str, int sigval)
{
	char field[20];
	size_t mem, mused;
	FILE *meminfo;
	int r;

	if ((meminfo = fopen(meminfofile, "r")) == NULL) {
		*str = '\0';
		return 1;
        }

	mused = 0;
	do
		if ((r = fscanf(meminfo, "%s %zu kB", field, &mem)) != 2)
			break;
                else if (memcmp(field, "MemTotal:", 8) == 0)
			mused += mem;
		else if (memcmp(field, "MemFree:", 8) == 0)
			mused -= mem;
		else if (memcmp(field, "Buffers:", 8) == 0)
			mused -= mem;
		else if (memcmp(field, "Cached:", 8) == 0)
			mused -= mem;
	while (memcmp(field, "KReclaimable:", 8) != 0);
	mused -= mem;

	fclose(meminfo);
	if (r != 2) {
		*str = '\0';
		return 1;
        }

	return SPRINTF(str, ICONmem " %zuMi", mused / 1024);
}

void
memc(int button)
{
	switch (button) {
	case 1:
		TERMCMD("htop", "-s", "PERCENT_CPU");
		break;
	case 2:
		TERMCMD("htop");
		break;
	case 3:
		TERMCMD("htop", "-s", "PERCENT_MEM");
		break;
	}
}
