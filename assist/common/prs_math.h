#if !defined(_ZZ_q_prs_math_DEFINED)
#define _ZZ_q_prs_math_DEFINED
/*===========================================================================*/

#include "cm_sys.h"

#ifdef PASCAL_SURE
#define RERR(ee,x,s,y) error(ee)
#define IERR(ee,x,s,y) error(ee)
#else
#define RERR(ee,x,s,y) real_eval_err(ee,x,s,y)
#define IERR(ee,x,s,y) int_eval_err(ee,x,s,y)
#endif


                      /********************************/
                      /********************************/
                      /****                        ****/
                      /****   INTEGER ARITHMETIC   ****/
                      /****                        ****/
                      /********************************/
                      /********************************/

#ifdef OMIT_MATH_CHECKS
#define iadd(x,y) ((x)+(y))
#define isub(x,y) ((x)-(y))
#define impy(x,y) ((x)*(y))
#define imod(x,y) ((x)%(y))
#define icyc(x,y) (1+(((x)-1)%(y)))
#define iquo(x,y) ((x)/(y))
#else
#define iadd(x,y) \
           (((((x)>0)&&((y)>(MAX_INT_TYPE-(x)))) || \
             (((x)<0)&&((y)<(-MAX_INT_TYPE-(x))))) \
                ? IERR(E_INTEGEROVF,x,"+",y) \
                : (x)+(y))
#define isub(x,y) iadd(x,-(y))
#define impy(x,y) \
          (((labs(y)>1)&&(labs(x)>=(1+(MAX_INT_TYPE-1)/labs(y)))) \
              ? IERR(E_INTEGEROVF,x,"*",y) \
              : ((x)*(y)))
#define imod(x,y) (((y)==0) ? IERR(E_IMOD0,x," MOD ",y) : ((x)%(y)))
#define icyc(x,y) (((y)==0) ? IERR(E_ICYC0,x," CYC ",y) : (1+(((x)-1)%(y))))
#define iquo(x,y) (((y)==0) ? IERR(E_IDVD0,x," DIV ",y) : ((x)/(y)))
#endif

                       /******************************/
                       /******************************/
                       /****                      ****/
                       /****   REAL  ARITHMETIC   ****/
                       /****                      ****/
                       /******************************/
                       /******************************/

#ifdef OMIT_MATH_CHECKS
#define rsqrt(x) sqrt(x)
#define rexp(x) exp(x)
#define rln(x) log(x)
#define rxtoy(x,y) exp((y)*log(x))
#define r_asin(x) asin(x)
#define r_acos(x) acos(x)
#else
#define rsqrt(x) (((x)<ZERO) ? RERR(E_NEGROOT,x,"<",ZERO) : sqrt(x))
#define rexp(x) (((x)>ln_maxreal) ? RERR(E_EXPOVF,x,">",ln_maxreal) : exp(x))
#define rln(x) (((x)<=MIN_REAL_TYPE) ? RERR(E_LNOFNEG,x,"<=",ZERO) : log(x))
#define rxtoy(x,y) \
        (((x)<=MIN_REAL_TYPE) ? RERR(E_NEGTOREAL,x,"**",y) \
                     : exp(rmpy((y),log(x))))
#define r_asin(x) ((fabs(x)>ONE) ? RERR(E_BADARCSIN,x,"(ABS)>",ONE) : asin(x))
#define r_acos(x) ((fabs(x)>ONE) ? RERR(E_BADARCCOS,x,"(ABS)>",ONE) : acos(x))
#endif


#ifdef OMIT_MATH_CHECKS
#define radd(x,y) ((x)+(y))
#define rsub(x,y) ((x)-(y))
#define rmpy(x,y) ((x)*(y))
#define rdvd(x,y) ((x)/(y))
#else
#define radd(x,y) \
           (((((x)>ZERO)&&((y)>(MAX_REAL_TYPE-(x)))) || \
             (((x)<ZERO)&&((y)<(-MAX_REAL_TYPE-(x))))) \
                ? RERR(E_REALOVF,x,"+",y) \
                : (x)+(y))
#define rsub(x,y) radd(x,(-(y)))
#define rmpy(x,y) \
          (((fabs(y)>ONE)&&(fabs(x)>(MAX_REAL_TYPE/fabs(y)))) \
              ? RERR(E_REALOVF,x,"*",y) \
              : ((x)*(y)))
#define rdvd(x,y) \
        (((fabs(y))<=MIN_REAL_TYPE) \
            ? RERR(E_RDVD0,x,"/",y) \
            : (((fabs(y)<ONE)&&(fabs(x)>(MAX_REAL_TYPE*fabs(y)))) \
                   ? RERR(E_REALOVF,x,"/",y) \
                   : ((x)/(y))))
#endif

/*===========================================================================*/
#endif
