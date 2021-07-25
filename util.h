#include "shared.h"

#define COL0                            "\x0b" /* default status color */
#define COL1                            "\x0c" /* default icon color */
#define COL2                            "\x0d" /* alternate icon color */
#define COL3                            "\x0e" /* mail block - syncing */
#define COL4                            "\x0f" /* mail block - frozen */

#define SCRIPT(name)                    "/home/ashish/.scripts/"name
#define TERMCMD(...)                    cspawn((char *[]){ "st", "-e", __VA_ARGS__, NULL })

#define SPRINTF(str, ...)               ({ \
                                                int len = snprintf(str, BLOCKLENGTH, __VA_ARGS__); \
                                                len < BLOCKLENGTH ? len + 1 : BLOCKLENGTH; \
                                        })

void cspawn(char *const *arg);
void csigself(int sig, int sigval);
size_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void uspawn(char *const *arg);
