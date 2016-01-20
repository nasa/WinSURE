#define SUN
/*
     The value of x=ARGLIMIT must satisfy the equation:

       (x-1.5)*(ln(x)-1.0)+x-1.5 <= ln(MAXREAL)

     If your system is not listed in this file, you may calculate
     appropriate values using the program "calclim".   For example:
          <run calclim program>
          Output file name: scratch.scr

*/

#if !defined(_ZZ_q_gammalim_DEFINED)
#define _ZZ_q_gammalim_DEFINED
/*===========================================================================*/

#include "cm_sys.h"

#ifdef CDC
#define ARGLIMIT 177.803
#define MACHINE_PRECISION 1.0e-60
#define INVERSE_MACHINE_PRECISION 1.0e+50
#define LN_MAX_REAL_TYPE 674.0
#define SQRT_MAX_REAL_TYPE 1.0e35
#endif

#ifdef PRIME
#define ARGLIMIT 34.844
#endif

#ifdef VAX
#define ARGLIMIT 2.7936006502397356e+01
#define MACHINE_PRECISION 2.7755575615628914e-17
#define INVERSE_MACHINE_PRECISION 3.6028797018963968e+16
#define LN_MAX_REAL_TYPE 8.8029691931113053e+01
#define SQRT_MAX_REAL_TYPE 1.3043817825332782e+19
#define PI 3.1415926535897932
#endif

#ifdef DEC
#define ARGLIMIT 2.793600639832757e+01
#define MACHINE_PRECISION 7.450580596923828e-09
#define INVERSE_MACHINE_PRECISION 1.342177280000000e+08
#define LN_MAX_REAL_TYPE 8.802969160317690e+01
#define SQRT_MAX_REAL_TYPE 1.304381777674077e+19
#define PI 3.141592653589793
#endif

#ifdef SUN
#define ARGLIMIT 1.442658089418170e+02
#define MACHINE_PRECISION 2.107342425544701e-08
#define INVERSE_MACHINE_PRECISION 4.745313281212578e+07
#define LN_MAX_REAL_TYPE 7.097827054146078e+02
#define SQRT_MAX_REAL_TYPE 1.340780778866838e+154
#define PI 3.141592653589793
#endif

#if !defined(ARGLIMIT)
#define ARGLIMIT 10.0
#endif

/*===========================================================================*/
#endif
