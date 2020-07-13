#define SLEEPINTERVAL			1

#include "blocks/battery.h"
#include "blocks/cputemp.h"
#include "blocks/calendar.h"
#include "blocks/hotspot.h"
#include "blocks/time.h"
#include "blocks/volume.h"

/* If interval of a block is set to 0, the block will only be updated once at startup.
 * If interval is set to a negative value, the block will never be updated in the main loop.
 * Set signal to 0 if signalling is not required for the block.
 * Keep signal less than 10 for clickable blocks.
 * If multiple realtime signals are pending, then the lowest numbered signal is delivered first. */

static Block blocks[] = {
/*	funcu			funcc			interval	signal */
	{ hotspotu,		hotspotc,		-1,		3 },

	{ timeu,		NULL,			30,		10 },

	{ calendaru,		calendarc,		60,		5 },

//	{ mailu,		mailc,			30,		2 },

	{ volumeu,		volumec,		0,		1 },

	{ cputempu,		cputempc,		1,		6 },

	{ batteryu,		batteryc,		10,		4 },

        { NULL } /* just to mark the end of the array */
};

static const char *delim =  "   ";
