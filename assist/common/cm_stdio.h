/*
    Symbols, some of which should be defined on command line:

              -DUNIX 

*/
#if !defined(_ZZ_q_cm_stdio_DEFINED)
#define _ZZ_q_cm_stdio_DEFINED
/*===========================================================================*/

                   /********************************/
                   /********************************/
                   /****                        ****/
                   /****   standard libraries   ****/
                   /****                        ****/
                   /********************************/
                   /********************************/

#include "lib_def.h"
#include "commonio.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef TIME_REQD
#include <time.h>
#endif

#ifdef MSDOS
#include <sys\types.h>
#include <sys\stat.h>
typedef struct stat stat_t;
#endif


#if defined(UNIX)||defined(LINUX)
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <stdlib.h>


/*===========================================================================*/
#endif
