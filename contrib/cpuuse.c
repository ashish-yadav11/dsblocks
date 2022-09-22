#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "cpuuse.h"

#define ICONcpu                      COL1 "\uf2db" COL0
static const char statfile[] = "/proc/stat";

size_t
cpuuseu(char *str, int sigval)
{
	static size_t cpunew[10];
	size_t cpuold[10];
	size_t idle, working;
	FILE *cpustat;
	int i, ret;

	if ((cpustat = fopen(statfile, "r")) == NULL) {
		*str = '\0';
		return 1;
	}

	memcpy(cpuold, cpunew, sizeof cpuold);
	ret = fscanf(cpustat, "cpu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu",
		     &cpunew[0], &cpunew[1], &cpunew[2], &cpunew[3], &cpunew[4],
		     &cpunew[5], &cpunew[6], &cpunew[7], &cpunew[8], &cpunew[9]);

	fclose(cpustat);
	if (ret != LENGTH(cpuold)) {
		*str = '\0';
		return 1;
	}

	idle = working = 0;
	for (i = 0; i < 3; ++i)
		working += cpunew[i] - cpuold[i];
	for (; i < 10; ++i)
		idle += cpunew[i] - cpuold[i];

	return SPRINTF(str, ICONcpu " %0.1f%%", working * 100.0 / (idle + working));
}

void
cpuusec(int button)
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
