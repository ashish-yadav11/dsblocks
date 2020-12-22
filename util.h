#include "shared.h"

#define COL0                            "\x0b" /* default status color */
#define COL1                            "\x0c" /* default icon color */
#define COL2                            "\x0d" /* alternate icon color */
#define COL3                            "\x0e" /* mail block - frozen */
#define COL4                            "\x0f" /* mail block - MAILSYNC started */
#define COL5                            "\x10" /* mail block - syncing */

#define LENGTH(X)                       (sizeof X / sizeof X[0])
#define SCRIPT(name)                    "/home/ashish/.scripts/"name
#define TERMCMD(cmd)                    cspawn((char *[]){ "/usr/bin/termite", "-e", cmd, NULL })

void cspawn(char *const *arg);
void csigself(int sig, int sigval);
size_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void uspawn(char *const *arg);
