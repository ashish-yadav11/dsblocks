#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "battery.h"

#define UDEVNOTRELIABLE

#define ICON0				COL1 "" COL0
#define ICON1				COL2 "" COL0
#define ICON2				COL1 "" COL0
#define ICON3				COL1 "" COL0
#define ICON4				COL1 "" COL0
#define ICON5				COL1 "" COL0

#define BATC				10  /* critical level */
#define BATL				20  /* low level */
#define BATP				40  /* warn to plug in charger below this level */
#define BATU				80  /* warn to unplug charger below this level */

#define BATCAPFILE			"/sys/class/power_supply/BAT0/capacity"
#define BATSTATEFILE			"/sys/class/power_supply/BAT0/status"
#define BATCFULLFILE			"/sys/class/power_supply/BAT0/charge_full"
#define BATCNOWFILE			"/sys/class/power_supply/BAT0/charge_now"
#define BATRATEFILE			"/sys/class/power_supply/BAT0/current_now"

#define UNNOTIFY(t, msg) \
	uspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "BatMon", msg, NULL })
#define UCNOTIFY(t, msg) \
	uspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "-u", "critical", "BatMon", msg, NULL })
#define CNNOTIFY(t, msg) \
	cspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "BatMon", msg, NULL })
#define CCNOTIFY(t, msg) \
	cspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "-u", "critical", "BatMon", msg, NULL })

enum { Normal, Critical, Low, Plug, Unplug }; /* level */
enum { Unknown, Discharging, Charging, Full, NotCharging }; /* state */

static int
chargingstatus()
{
        int c;
        FILE *fp;

        if (!(fp = fopen(BATSTATEFILE, "r")))
                return Unknown;
        if ((c = fgetc(fp)) == EOF) {
                fclose(fp);
                return Unknown;
        }
        fclose(fp);
        if (c == 'D') /* Discharging */
                return Discharging;
        else if (c == 'C') /* Charging */
                return Charging;
        else if (c == 'F') /* Full */
                return Full;
        else if (c == 'U') /* Unknown */
                return Unknown;
        else /* Not Charging */
                return NotCharging;
}

void
batteryu(char *str, int ac)
{
#ifdef UDEVNOTRELIABLE
        static int charging = -1;
#endif
        static int level = Normal;
        int bat;

        if (!readint(BATCAPFILE, &bat)) {
                strcpy(str, ICON0);
                return;
        }
        if (ac == NILL) {
                switch (chargingstatus()) {
                        case Charging:
                        case Full:
#ifdef UDEVNOTRELIABLE
                                if (charging != 1) {
                                        if (charging != -1 && bat < BATU)
                                                UNNOTIFY("1000", "Charger plugged in");
                                        charging = 1;
                                }
#endif
                                goto onac;
                        default:
#ifdef UDEVNOTRELIABLE
                                if (charging != 0) {
                                        if (charging != -1 && bat > BATP)
                                                UNNOTIFY("1000", "Charger plugged out");
                                        charging = 0;
                                }
#endif
                                goto onbat;
                }
        } else {
                if (ac) {
                        if (bat < BATU)
                                UNNOTIFY("1000", "Charger plugged in");
#ifdef UDEVNOTRELIABLE
                        charging = 1;
#endif
                        goto onac;
                } else {
                        if (bat > BATP)
                                UNNOTIFY("1000", "Charger plugged out");
#ifdef UDEVNOTRELIABLE
                        charging = 0;
#endif
                        goto onbat;
                }
        }
onac:
        snprintf(str, CMDLENGTH, ICON5 "%d%%", bat);
        if (bat >= BATU) {
                if (level != Unplug) {
                        UNNOTIFY("0", "Unplug the charger");
                        level = Unplug;
                }
        } else if (level != Normal)
                level = Normal;
        return;
onbat:
        if (bat >= BATU) {
                snprintf(str, CMDLENGTH, ICON4 "%d%%", bat);
                if (level != Normal)
                        level = Normal;
        } else if (bat > BATP) {
                snprintf(str, CMDLENGTH, ICON3 "%d%%", bat);
                if (level != Normal)
                        level = Normal;
        } else if (bat > BATL) {
                snprintf(str, CMDLENGTH, ICON2 "%d%%", bat);
                if (level != Plug) {
                        UNNOTIFY("0", "Plug in the charger");
                        level = Plug;
                }
        } else {
                snprintf(str, CMDLENGTH, ICON1 "%d%%", bat);
                if (bat > BATC) {
                        if (level != Low) {
                                UNNOTIFY("0", "Battery level is low!");
                                level = Low;
                        }
                } else {
                        if (level != Critical) {
                                UCNOTIFY("0", "Battery level is critical!");
                                level = Critical;
                        }
                }
        }
}

void
batteryc(int button)
{
        int cur, rate;
        int hr, mn;

        switch (chargingstatus()) {
                case Unknown:
                        CCNOTIFY("0", "Battery status is unknown!");
                        return;
                case Full:
                        CNNOTIFY("2000", "Battery fully charged");
                        return;
                case Discharging:
                case NotCharging:
                        if (!readint(BATCNOWFILE, &cur)) {
                                CCNOTIFY("0", "Couldn't read" BATCNOWFILE);
                                return;
                        }
                        break;
                case Charging:
                {
                        int cnow;

                        if (!readint(BATCFULLFILE, &cur)) {
                                CCNOTIFY("0", "Couldn't read" BATCFULLFILE);
                                return;
                        }
                        if (!readint(BATCNOWFILE, &cnow)) {
                                CCNOTIFY("0", "Couldn't read" BATCNOWFILE);
                                return;
                        }
                        cur -= cnow;
                }
        }
        if (!readint(BATRATEFILE, &rate)) {
                CCNOTIFY("0", "Couldn't read" BATRATEFILE);
                return;
        }
        if (!rate) {
                CCNOTIFY("0", "Unexpected current value!");
                return;
        }
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
