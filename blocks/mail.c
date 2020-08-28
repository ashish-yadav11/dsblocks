#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "mail.h"

#define ICON0                           COL3 "" COL0 /* frozen */
#define ICON1                           COL4 "" COL0 /* MAILSYNC started */
#define ICON2                           COL5 "" COL0 /* syncing */
#define ICON3                           COL1 "" COL0 /* last sync successfull */
#define ICON4                           COL2 "" COL0 /* last sync failed */

#define NEWMAILDIR                      "/home/ashish/.local/share/mail/iiser/INBOX/new"

#define MAILSYNC                        (char *[]){ "/home/ashish/.scripts/mailsync.sh", NULL }

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
        static int frozen;
        static int syncing;

        if (sigval == NILL) {
                if (!frozen)
                        uspawn(MAILSYNC);
        } else {
                if (sigval > 0) {
                        if ((n = numnewmails()) < 0) {
                                *str = '\0';
                                return;
                        }
                        /* MAILSYNC started */
                        if (sigval == 1) {
                                /* unfreeze if frozen */
                                if (frozen)
                                        frozen = 0;
                                /* syncing is in progress in another instance of MAILSYNC */
                                if (syncing) {
                                        syncing = -1;
                                        snprintf(str, CMDLENGTH, ICON2 "%d", n);
                                } else
                                        snprintf(str, CMDLENGTH, ICON1 "%d", n);
                                return;
                        } else
                                if (frozen) {
                                        snprintf(str, CMDLENGTH, ICON0 "%d", n);
                                        return;
                                }
                } else if (sigval < 0) {
                        if (n < 0)
                                return;
                        if (frozen)
                                return;
                        sigval = -sigval;
                } else {
                        if (frozen) {
                                frozen = 0;
                                uspawn(MAILSYNC);
                        } else {
                                if (n >= 0)
                                        snprintf(str, CMDLENGTH, ICON0 "%d", n);
                                frozen = 1;
                        }
                        return;
                }
                switch (sigval) {
                        /* MAILSYNC started */
                        case 1:
                                snprintf(str, CMDLENGTH, ICON1 "%d", n);
                                break;
                        /* sync started */
                        case 2:
                                snprintf(str, CMDLENGTH, ICON2 "%d", n);
                                syncing = 1;
                                break;
                        /* sync successfull */
                        case 3:
                                if (syncing > 0)
                                        snprintf(str, CMDLENGTH, ICON3 "%d", n);
                                /* the other instance of MAILSYNC is going to ping */
                                else
                                        snprintf(str, CMDLENGTH, ICON1 "%d", n);
                                syncing = 0;
                                break;
                        /* sync failed */
                        case 4:
                                if (syncing > 0)
                                        snprintf(str, CMDLENGTH, ICON4 "%d", n);
                                /* the other instance of MAILSYNC is going to ping */
                                else
                                        snprintf(str, CMDLENGTH, ICON1 "%d", n);
                                syncing = 0;
                                break;
                        /* ping failed */
                        case 5:
                                snprintf(str, CMDLENGTH, ICON4 "%d", n);
                                break;
                }
        }
}

void
mailc(int button)
{
        switch (button) {
                case 1:
                        cspawn(MAILSYNC);
                        break;
                case 3:
                        csigself(2, 0);
                        break;
        }
}
