#include "shared.h"

#define COL0                            "" /* default status color */
#define COL1                            "" /* default icon color */
#define COL2                            "\r" /* alternate icon color */
#define COL3                            "" /* mail block - frozen */
#define COL4                            "" /* mail block - MAILSYNC started */
#define COL5                            "" /* mail block - syncing */

#define LENGTH(X)                       (sizeof X / sizeof X[0])
#define SCRIPT(name)                    "/home/ashish/.scripts/"name
#define TERMCMD(cmd)                    cspawn((char *[]){ "/usr/bin/termite", "-e", cmd, NULL })

void cspawn(char *const *arg);
void csigself(int sig, int sigval);
ssize_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void uspawn(char *const *arg);
