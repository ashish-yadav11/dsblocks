#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../util.h"
#include "mail.h"

#define ICONn                           COL1 "" COL0 /* last sync successful */
#define ICONe                           COL2 "" COL0 /* last sync failed */
#define ICONs                           COL3 "" COL0 /* syncing */
#define ICONz                           COL4 "" COL0 /* frozen */

#define NEWMAILDIR                      "/home/ashish/.local/share/mail/iiser/INBOX/new"

#define MAILSYNC                        (char *[]){ SCRIPT("mailsync.sh"), NULL }

static int
countnewmails()
{
        static time_t lastmtime = 0;
        struct stat buf;

        if (stat(NEWMAILDIR, &buf) == -1)
                return -1;
        if (buf.st_mtime == lastmtime)
                return 0;
        lastmtime = buf.st_mtime;
        return 1;
}

static int
numnewmails()
{
        int n;
        DIR* dir;
        struct dirent* entry;

        if (!(dir = opendir(NEWMAILDIR)))
                return -1;
        n = 0;
        while ((entry = readdir(dir)))
                if (entry->d_type == DT_REG)
                        n++;
        closedir(dir);
        return n;
}

int
mailu(char *str, int sigval)
{
        static int n;
        static int frozen;

        /* routine update */
        if (sigval == NILL) {
                if (!frozen)
                        uspawn(MAILSYNC);
                return 0;
        /* handle signals from MAILSYNC */
        } else if (sigval >= 0) {
                if (n < 0)
                        n = numnewmails();
                else {
                         switch (countnewmails()) {
                                 case -1:
                                         n = -1;
                                         break;
                                 case 1:
                                         n = numnewmails();
                                         break;
                         }
                }
                if (n < 0) {
                        *str = '\0';
                        return 1;
                }
                /* MAILSYNC started */
                if (sigval == 0) {
                        frozen = 0;
                        snprintf(str, BLOCKLENGTH, ICONs "%d", n);
                /* sync finished */
                } else {
                        if (frozen)
                                snprintf(str, BLOCKLENGTH, ICONz "%d", n);
                        else if (sigval == 1)
                                snprintf(str, BLOCKLENGTH, ICONn "%d", n);
                        else
                                snprintf(str, BLOCKLENGTH, ICONe "%d", n);
                }
        /* toggle frozen */
        } else {
                if (n < 0)
                        return 0;
                if (frozen) {
                        uspawn(MAILSYNC);
                        return 0;
                } else {
                        frozen = 1;
                        snprintf(str, BLOCKLENGTH, ICONz "%d", n);
                }
        }
        return 1;
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
