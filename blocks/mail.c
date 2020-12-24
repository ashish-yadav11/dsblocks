#include <dirent.h>
#include <stdio.h>

#include "../util.h"
#include "mail.h"

#define ICONn                           COL1 "" COL0 /* last sync successful */
#define ICONe                           COL2 "" COL0 /* last sync failed */
#define ICONs                           COL3 "" COL0 /* syncing */
#define ICONz                           COL4 "" COL0 /* frozen */

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

        /* routine update */
        if (sigval == NILL) {
                if (!frozen)
                        uspawn(MAILSYNC);
        /* sync finished */
        } else if (sigval > 0) {
                if ((n = numnewmails()) < 0) {
                        *str = '\0';
                        return;
                }
                if (frozen)
                        snprintf(str, BLOCKLENGTH, ICONz "%d", n);
                else if (sigval == 1)
                        snprintf(str, BLOCKLENGTH, ICONn "%d", n);
                else
                        snprintf(str, BLOCKLENGTH, ICONe "%d", n);
        } else {
                if (n < 0)
                        return;
                /* sync started */
                if (sigval == 0) {
                        if (!frozen)
                                snprintf(str, BLOCKLENGTH, ICONs "%d", n);
                /* toggle frozen */
                } else {
                        if (frozen) {
                                frozen = 0;
                                uspawn(MAILSYNC);
                        } else {
                                frozen = 1;
                                snprintf(str, BLOCKLENGTH, ICONz "%d", n);
                        }
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
                        csigself(3, -1);
                        break;
        }
}
