#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../util.h"
#include "battery.h"

#define BATCAPFILE			"/sys/class/power_supply/BAT0/capacity"
#define	ACSTATEFILE			"/sys/class/power_supply/AC/online"
#define BATRATEFILE			"/sys/class/power_supply/BAT0/current_now"
#define BATCNOWFILE			"/sys/class/power_supply/BAT0/charge_now"
#define BATCFULLFILE			"/sys/class/power_supply/BAT0/charge_full"

#define UNNOTIFY(t, msg) \
	uspawn((char *[]) { "/usr/bin/dunstify", "-r", "2120", "-t", t, "BatMon", msg, NULL })
#define UCNOTIFY(t, msg) \
	uspawn((char *[]) { "/usr/bin/dunstify", "-r", "2120", "-t", t, "-u", "critical", "BatMon", msg, NULL })
#define CNNOTIFY(t, msg) \
	cspawn((char *[]) { "/usr/bin/dunstify", "-r", "2120", "-t", t, "BatMon", msg, NULL })

enum { Normal, Critical, Low, Plug, Unplug };

static int state;

void
batteryu(char *str, int ac)
{
        int bat;

        if (!readint(BATCAPFILE, &bat)) {
                *str = '\0';
                return;
        }
        if (ac == NILL) {
                if (!readint(ACSTATEFILE, &ac)) {
                        *str = '\0';
                        return;
                }
        } else {
                if (ac) {
                        UNNOTIFY("1000", "Charger plugged in");
                        goto yac;
                } else {
                        UNNOTIFY("1000", "Charger plugged out");
                        goto nac;
                }
        }
        if (ac) {
yac:
                snprintf(str, CMDLENGTH, BAT5 "%d%%", bat);
                if (bat >= 80) { 
                        if (state != Unplug) {
                                state = Unplug;
                                UNNOTIFY("0", "Unplug the charger");
                        }
                } else if (state != Normal)
                        state = Normal;
        } else {
nac:
                if (bat >= 80) {
                        snprintf(str, CMDLENGTH, BAT4 "%d%%", bat);
                        if (state != Normal)
                                state = Normal;
                } else if (bat > 40) {
                        snprintf(str, CMDLENGTH, BAT3 "%d%%", bat);
                        if (state != Normal)
                                state = Normal;
                } else if (bat > 20) {
                        snprintf(str, CMDLENGTH, BAT2 "%d%%", bat);
                        if (state != Plug) {
                                state = Plug;
                                UNNOTIFY("0", "Plug in the charger");
                        }
                } else {
                        snprintf(str, CMDLENGTH, BAT1 "%d%%", bat);
                        if (bat > 10) {
                                if (state != Low) {
                                        state = Low;
                                        UNNOTIFY("0", "Battery level is low!");
                                }
                        } else {
                                if (state != Critical) {
                                        state = Critical;
                                        UCNOTIFY("0", "Battery level is critical!");
                                }
                                if (bat <= 5)
                                        uspawn((char *[]) { "/usr/bin/systemctl", "hibernate", NULL });
                        }
                }
        }
}

void
batteryc(int button)
{
        int cur, rate;
        int ac, cnow, cfull;
        int hr, mn;

        if (!readint(BATRATEFILE, &rate))
                return;
        if (!rate)
                CNNOTIFY("2000", "Battery fully charged");
        if (!readint(BATCNOWFILE, &cnow))
                return;
        if (!readint(ACSTATEFILE, &ac))
                return;
        if (ac) {
                if (!readint(BATCFULLFILE, &cfull))
                        return;
                cur = cfull - cnow;
        } else
                cur = cnow;
        hr = cur / rate;
        mn = ((cur * 60) / rate) - hr * 60;
        switch (hr) {
                case 0:
                        switch (mn) {
                                case 0:
                                        CNNOTIFY("2000", "Battery fully charged");
                                        break;
                                case 1:
                                        CNNOTIFY("2000", "1 minute remaining");
                                        break;
                                default:
                                {
                                        char buf[21];
                                        snprintf(buf, sizeof buf, "%d minutes remaining", mn);
                                        CNNOTIFY("2000", buf);
                                }
                                        break;
                        }
                        break;
                case 1:
                        switch (mn) {
                                case 0:
                                        CNNOTIFY("2000", "1 hour remaining");
                                        break;
                                case 1:
                                        CNNOTIFY("2000", "1 hour, 1 minute remaining");
                                        break;
                                default:
                                {
                                        char buf[29];
                                        snprintf(buf, sizeof buf, "1 hour, %d minutes remaining", mn);
                                        CNNOTIFY("2000", buf);
                                }
                                        break;
                        }
                        break;
                default:
                        switch (mn) {
                                case 0:
                                {
                                        char buf[19];
                                        snprintf(buf, sizeof buf, "%d hours remaining", hr);
                                        CNNOTIFY("2000", buf);
                                }
                                        break;
                                case 1:
                                {
                                        char buf[29];
                                        snprintf(buf, sizeof buf, "%d hours, 1 minute remaining", hr);
                                        CNNOTIFY("2000", buf);
                                }
                                        break;
                                default:
                                {
                                        char buf[31];
                                        snprintf(buf, sizeof buf, "%d hours, %d minutes remaining", hr, mn);
                                        CNNOTIFY("2000", buf);
                                }
                                        break;
                        }
                        break;
        }
}
