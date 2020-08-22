#include "shared.h"

#define COL0                            "" /* default status color */
#define COL1                            "" /* default icon color */
#define COL2                            "\r" /* warning icon color */
#define COL3                            "" /* mail block - frozen */
#define COL4                            "" /* mail block - MAILSYNC started */
#define COL5                            "" /* mail block - syncing */

#define TERMCMD(cmd)                    cspawn((char *[]){ "/usr/bin/termite", "-e", cmd, NULL })

void cspawn(char *const *arg);
void csigself(int signal, int sigval);
ssize_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void uspawn(char *const *arg);
