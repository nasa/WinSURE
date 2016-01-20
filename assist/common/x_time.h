/*
   Definitions which are missing from "time.h" on archaic systems such
   as SUN and ULTRIX
*/

#if !defined(_ZZ_q_x_time_DEFINED)
#define _ZZ_q_x_time_DEFINED

#include "cm_sys.h"

extern time_t time(time_t *);
extern clock_t clock(void);

#endif
