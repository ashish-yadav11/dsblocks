#include "shared.h"

#define COL0			""
#define COL1			""
#define COL2			""
#define COL3			""
#define COL4			""
#define COL5			""
#define COL6			""
#define COL7			""
#define COL8			""

ssize_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void cspawn(char *const *arg);
void uspawn(char *const *arg);
