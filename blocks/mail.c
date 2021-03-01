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

static int newmails;

static void
updatenewmails()
{
        static time_t lastmtime = 0;
        DIR* dir;
        struct dirent* entry;
        struct stat buf;

        if (stat(NEWMAILDIR, &buf) == -1) {
                newmails = -1;
                return;
        }
        if (buf.st_mtime == lastmtime)
                return;
        lastmtime = buf.st_mtime;

        if (!(dir = opendir(NEWMAILDIR))) {
                newmails = -1;
                return;
        }
        newmails = 0;
        while ((entry = readdir(dir)))
                if (entry->d_type == DT_REG)
                        newmails++;
        closedir(dir);
}

size_t
mailu(char *str, int sigval)
{
        static int frozen;

        /* update newmails on signals from MAILSYNC */
        if (sigval >= -1) {
                updatenewmails();
                if (newmails < 0) {
                        *str = '\0';
                        return 1;
                }
        }
        switch (sigval) {
                /* routine update */
                case NILL:
                        if (!frozen)
                                uspawn(MAILSYNC);
                        return 0;
                /* toggle frozen */
                case -2:
                        if (newmails < 0)
                                return 0;
                        if (frozen) {
                                uspawn(MAILSYNC);
                                return 0;
                        } else {
                                frozen = 1;
                                return SPRINTF(str, ICONz "%d", newmails);
                        }
                /* MAILSYNC started */
                case -1:
                        frozen = 0;
                        return SPRINTF(str, ICONs "%d", newmails);
                /* sync successful */
                case 0:
                        if (frozen)
                                return SPRINTF(str, ICONz "%d", newmails);
                        else
                                return SPRINTF(str, ICONn "%d", newmails);
                /* sync failed */
                default:
                        if (frozen)
                                return SPRINTF(str, ICONz "%d", newmails);
                        else
                                return SPRINTF(str, ICONe "%d", newmails);
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
                        csigself(3, -2);
                        break;
        }
}
