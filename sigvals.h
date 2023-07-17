#include <limits.h>

#define STRT                            INT_MIN     /* first run of blocks */
#define RTNE                            (INT_MIN+1) /* routine updates in the loop */
#define NONE                            (INT_MIN+2) /* no sigval argument from dsblocks */

#define ISSPLSIGVAL(sigval)             (sigval <= NONE)
