#include <dirent.h>
#include <stdio.h>

#include "../util.h"
#include "mail.h"

#define ICON0                           COL3 "" COL0 /* frozen */
#define ICON1                           COL4 "" COL0 /* MAILSYNC started */
#define ICON2                           COL5 "" COL0 /* syncing */
#define ICON3                           COL1 "" COL0 /* last sync successful */
#define ICON4                           COL2 "" COL0 /* last sync failed */

#define NEWMAILDIR                      "/home/ashish/.local/share/mail/iiser/INBOX/new"

#define MAILSYNC                        (char *[]){ SCRIPT("mailsync.sh"), NULL }

static int
numnewmails()
{
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

        /* routine update */
        if (sigval == NILL) {
                if (!frozen)
                        uspawn(MAILSYNC);
        /* toggle frozen */
        } else if (sigval == 0) {
                if (frozen) {
                        frozen = 0;
                        uspawn(MAILSYNC);
                } else {
                        frozen = 1;
                        if (n >= 0)
                                snprintf(str, BLOCKLENGTH, ICON0 "%d", n);
                }
        /* handle signals from MAILSYNC */
        } else {
                /* update mail count */
                if (sigval > 0) {
                        if ((n = numnewmails()) < 0) {
                                *str = '\0';
                                return;
                        }
                /* don't update mail count */
                } else {
                        if (n < 0)
                                return;
                        sigval = -sigval;
                }
                switch (sigval) {
                        /* MAILSYNC started */
                        case 1:
                                /* unfreeze if frozen */
                                if (frozen)
                                        frozen = 0;
                                /* syncing is in progress in another instance of MAILSYNC */
                                if (syncing) {
                                        snprintf(str, BLOCKLENGTH, ICON2 "%d", n);
                                        syncing = -1;
                                } else
                                        snprintf(str, BLOCKLENGTH, ICON1 "%d", n);
                                break;
                        /* sync started */
                        case 2:
                                if (frozen)
                                        snprintf(str, BLOCKLENGTH, ICON0 "%d", n);
                                else
                                        snprintf(str, BLOCKLENGTH, ICON2 "%d", n);
                                syncing = 1;
                                break;
                        /* sync successful */
                        case 3:
                                if (frozen)
                                        snprintf(str, BLOCKLENGTH, ICON0 "%d", n);
                                else if (syncing > 0)
                                        snprintf(str, BLOCKLENGTH, ICON3 "%d", n);
                                /* the other instance of MAILSYNC was waiting to ping */
                                else
                                        snprintf(str, BLOCKLENGTH, ICON1 "%d", n);
                                syncing = 0;
                                break;
                        /* sync failed */
                        case 4:
                                if (frozen)
                                        snprintf(str, BLOCKLENGTH, ICON0 "%d", n);
                                else if (syncing > 0)
                                        snprintf(str, BLOCKLENGTH, ICON4 "%d", n);
                                /* the other instance of MAILSYNC was waiting to ping */
                                else
                                        snprintf(str, BLOCKLENGTH, ICON1 "%d", n);
                                syncing = 0;
                                break;
                        /* ping failed */
                        case 5:
                                if (frozen)
                                        snprintf(str, BLOCKLENGTH, ICON0 "%d", n);
                                else
                                        snprintf(str, BLOCKLENGTH, ICON4 "%d", n);
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
                        csigself(3, 0);
                        break;
        }
}
