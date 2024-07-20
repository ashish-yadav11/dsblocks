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

#define NOTIFY(t, msg)                  uspawn((char *[]){ \
                "notify-send", \
                "-h", "string:x-canonical-private-synchronous:mailsync", \
                "-t", t, \
                "-e", \
                "MailSync", msg, NULL })

static int newmails = 0;

static void
updatenewmails(void)
{
        static time_t lastmtime = 0;
        int prevnewmails;
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
        prevnewmails = newmails;
        newmails = 0;
        while ((entry = readdir(dir)))
                if (entry->d_type == DT_REG)
                        newmails++;
        closedir(dir);
        if (newmails > prevnewmails)
                NOTIFY("2000", "You have got new mails!");
}

size_t
mailu(char *str, int sigval)
{
        static int frozen;

        /* update newmails except for routine resync and when MAILSYNC signals
         * it's launch */
        if (sigval != RTNE && sigval != -1)
                updatenewmails();
        if (newmails < 0) {
                *str = '\0';
                return 1;
        }
        if (frozen && sigval != -2)
                return SPRINTF(str, ICONz "%d", newmails);
        switch (sigval) {
                /* routine update or `sigdsblocks 3` */
                case STRT:
                case RTNE:
                case NONE:
                        uspawn(MAILSYNC);
                        /* MAILSYNC will signal -1, no need to update text now */
                        return 0;
                /* toggle frozen */
                case -2:
                        if (frozen) {
                                frozen = 0;
                                uspawn(MAILSYNC);
                                return 0;
                        } else {
                                frozen = 1;
                                return SPRINTF(str, ICONz "%d", newmails);
                        }
                /* MAILSYNC started */
                case -1:
                        return SPRINTF(str, ICONs "%d", newmails);
                /* sync successful */
                case 0:
                        return SPRINTF(str, ICONn "%d", newmails);
                /* sync failed */
                default:
                        return SPRINTF(str, ICONe "%d", newmails);
        }
}

void
mailc(int button)
{
        switch (button) {
                case 1:
                        csigself(3, NONE);
                        break;
                case 3:
                        csigself(3, -2);
                        break;
        }
}
