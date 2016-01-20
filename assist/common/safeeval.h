#if !defined(_ZZ_q_safeeval_DEFINED)
#define _ZZ_q_safeeval_DEFINED

#include "cm_sys.h"

#ifdef _NO_PROTO
extern void eval_err();
extern real_type real_eval_err();
extern int_type int_eval_err();
extern int_type ixtoy();
extern real_type rtoi();
#else
extern void eval_err(short,char *);
extern real_type real_eval_err(short,real_type,char *,real_type);
extern int_type int_eval_err(short,int_type,char *,int_type);
extern int_type ixtoy(int_type,int_type);
extern real_type rtoi(real_type,int_type);
#endif

#endif
