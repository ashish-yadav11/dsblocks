#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "battery.h"

#define ICONe                           COL2 "" COL0 /* unexpected error */
#define ICONa                           COL1 "" COL0 /* no battery */

#define PDN                             COL1
#define PUP                             COL2

#define ICON0                           "" COL0
#define ICON1                           "" COL0
#define ICON2                           "" COL0
#define ICON3                           "" COL0
#define ICON4                           "" COL0
#define ICON5                           "" COL0
#define ICON6                           "" COL0
#define ICON7                           "" COL0
#define ICON8                           "" COL0
#define ICON9                           "" COL0

#define ICON(bat)                       icons[(bat * sizeof icons) / (101 * sizeof icons[0])]

#define BATC                            10 /* critical level */
#define BATL                            20 /* low level */
#define BATP                            40 /* plug in level */
#define BATU                            80 /* unplug level */

#define BATCAPFILE                      "/sys/class/power_supply/BAT0/capacity"
#define ACSTATEFILE                     "/sys/class/power_supply/AC/online"
#define BATCFULLFILE                    "/sys/class/power_supply/BAT0/charge_full"
#define BATCNOWFILE                     "/sys/class/power_supply/BAT0/charge_now"
#define BATRATEFILE                     "/sys/class/power_supply/BAT0/current_now"

#define UNNOTIFY(t, msg) \
        uspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "BatMon", msg, NULL })
#define UCNOTIFY(t, msg) \
        uspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "-u", "critical", "BatMon", msg, NULL })
#define CNNOTIFY(t, msg) \
        cspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "BatMon", msg, NULL })
#define CCNOTIFY(t, msg) \
        cspawn((char *[]){ "/usr/bin/dunstify", "-r", "2120", "-t", t, "-u", "critical", "BatMon", msg, NULL })

enum { Normal, Critical, Low, Plug, Unplug };

void
batteryu(char *str, int ac)
{
        static int level = Normal;
        static char *icons[] = { ICON0, ICON1, ICON2, ICON3, ICON4,
                                 ICON5, ICON6, ICON7, ICON8, ICON9 };
        int bat;

        if (!readint(BATCAPFILE, &bat)) {
                strcpy(str, ICONa);
                return;
        }
        /* routine update */
        if (ac == NILL) {
                if (!readint(ACSTATEFILE, &ac)) {
                        snprintf(str, CMDLENGTH, ICONe "%d%%", bat);
                        return;
                }
                if (ac) {
                        if (bat < BATU)
                                level = Normal;
                        else {
                                if (level != Unplug) {
                                        UNNOTIFY("0", "Unplug the charger");
                                        level = Unplug;
                                }
                        }
                        snprintf(str, CMDLENGTH, PUP "%s%d%%", ICON(bat), bat);
                } else {
                        if (bat > BATP)
                                level = Normal;
                        else if (bat > BATL) {
                                if (level != Plug) {
                                        UNNOTIFY("0", "Plug in the charger");
                                        level = Plug;
                                }
                        } else if (bat > BATC) {
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
                        snprintf(str, CMDLENGTH, PDN "%s%d%%", ICON(bat), bat);
                }
        /* charger plugged in */
        } else if (ac) {
                if (bat < BATU) {
                        UNNOTIFY("1000", "Charger plugged in");
                        level = Normal;
                } else {
                        UNNOTIFY("0", "Unplug the charger");
                        level = Unplug;
                }
                snprintf(str, CMDLENGTH, PUP "%s%d%%", ICON(bat), bat);
        /* charger plugged out */
        } else {
                if (bat > BATP) {
                        UNNOTIFY("1000", "Charger plugged out");
                        level = Normal;
                } else if (bat > BATL) {
                        UNNOTIFY("0", "Plug in the charger");
                        level = Plug;
                } else if (bat > BATC) {
                        UNNOTIFY("0", "Battery level is low!");
                        level = Low;
                } else {
                        UCNOTIFY("0", "Battery level is critical!");
                        level = Critical;
                }
                snprintf(str, CMDLENGTH, PDN "%s%d%%", ICON(bat), bat);
        }
}

void
batteryc(int button)
{
        int ac;
        int cur, rate;
        int hr, mn;

        if (!readint(ACSTATEFILE, &ac)) {
                CCNOTIFY("0", "Error: couldn't read " ACSTATEFILE);
                return;
        }
        if (ac) {
                int cnow;

                if (!readint(BATCFULLFILE, &cur)) {
                        CNNOTIFY("1000", "On AC power");
                        return;
                }
                if (!readint(BATCNOWFILE, &cnow)) {
                        CNNOTIFY("1000", "On AC power");
                        return;
                }
                cur -= cnow;
                if (!readint(BATRATEFILE, &rate)) {
                        CNNOTIFY("1000", "On AC power");
                        return;
                }
        } else {
                if (!readint(BATCNOWFILE, &cur)) {
                        CCNOTIFY("0", "Error: couldn't read " BATCNOWFILE);
                        return;
                }
                if (!readint(BATRATEFILE, &rate)) {
                        CCNOTIFY("0", "Error: couldn't read " BATRATEFILE);
                        return;
                }
        }
        if (!rate) {
                CNNOTIFY("2000", "Battery fully charged");
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
