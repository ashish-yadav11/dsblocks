#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "mail.h"

#define ICON0				COL7  "" COL0 /* frozen */
#define ICON1				COL8  "" COL0 /* MAILSYNC started */
#define ICON2				COL9  "" COL0 /* syncing */
#define ICON3				COL10 "" COL0 /* last sync successfull */
#define ICON4				COL11 "" COL0 /* last sync failed */

#define NEWMAILDIR			"/home/ashish/.local/share/mail/iiser/INBOX/new"

#define MAILSYNC			(char *[]){ "/home/ashish/.scripts/mailsync.sh", NULL }

static int frozen;

static int
numnewmails() {
        int n = 0;
        DIR* dir;
        struct dirent* rf;

        if (!(dir = opendir(NEWMAILDIR)))
                return -1;
        while ((rf = readdir(dir)))
                if (rf->d_type == DT_REG)
                        n++;
        closedir(dir);
        return n;
}

void
mailu(char *str, int sigval)
{
        static int n;
        static int syncing;

        if (frozen) {
                if (frozen == 1) {
                        if (n >= 0)
                                snprintf(str, CMDLENGTH, ICON0 "%d", n);
                        frozen = 2;
                }
                return;
        }
        if (sigval != NILL) {
                if (sigval > 0) {
                        if ((n = numnewmails()) < 0) {
                                *str = '\0';
                                return;
                        }
                        /* syncing is in progress and another instance of MAILSYNC started */
                        if (syncing && sigval == 1) {
                                snprintf(str, CMDLENGTH, ICON2 "%d", n);
                                return;
                        }
                } else {
                        if (n < 0)
                                return;
                        sigval = -sigval;
                }
                switch (sigval) {
                        /* MAILSYNC started */
                        case 1:
                                snprintf(str, CMDLENGTH, ICON1 "%d", n);
                                break;
                        /* sync started */
                        case 2:
                                syncing = 1;
                                snprintf(str, CMDLENGTH, ICON2 "%d", n);
                                break;
                        /* sync successfull */
                        case 3:
                                syncing = 0;
                                snprintf(str, CMDLENGTH, ICON3 "%d", n);
                                break;
                        /* sync failed */
                        case 4:
                                syncing = 0;
                        /* ping failed */
                        /* fall through */
                        case 5:
                                snprintf(str, CMDLENGTH, ICON4 "%d", n);
                                break;
                }
        } else
                uspawn(MAILSYNC);
}

void
mailc(int button)
{
        switch (button) {
                case 1:
                        if (frozen)
                                frozen = 0;
                        cspawn(MAILSYNC);
                        break;
                case 3:
                        if (frozen)
                                frozen = 0;
                        else
                                frozen = 1;
                        csigself(2, NILL);
                        break;
        }
}
