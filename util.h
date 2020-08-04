#include "shared.h"

#define COL0			""
#define COL1			""
#define COL2			"\r"
#define COL3			""
#define COL4			""
#define COL5			""

void cspawn(char *const *arg);
void csigself(int signal, int sigval);
ssize_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void uspawn(char *const *arg);
