#include "main_def.h"
#include "cm_sys.h"
#include "cm_defs.h"

#if defined(UNIX)||defined(ULTRIX)
#define DBL_EPSILON X_EPS
#endif
#include "gamma.h"


#define ln log

extern int main(void);
int main()
{
   char filename[130];
   char fmt[90];
   Boolean repeat;
   real_type xlow,xhigh,x,eps,pi;
   static real_type approx_one = (1.0000000000000000 - DBL_EPSILON);
   printf("Output file name: ");
   gets(filename);
   if (((*filename)!='\0')&&((*filename)!=' ')) freopen(filename,"w",stdout);
   xlow=0.0000000000;
   xhigh=20000.0000000000;
   eps=exp(-((real_type) (REAL_DEC_PLACES-1))*ln(10.0000000000000000));
   sprintf(fmt,"#define %%s %%%d.%d%s\n",
           REAL_DEC_PLACES,REAL_DEC_PLACES,EXP_TYPE_FORMAT);
   for (repeat=TRUE;repeat;){
      x=(xlow+xhigh)/2.00000000000;
      if (((x-1.5)*(ln(x)-1.0)+x-1.5)<=ln(MAX_REAL_TYPE)) xlow=x;
      else xhigh=x;
      if ((xhigh-xlow)<(x*eps)) repeat=FALSE;}
   x *= approx_one;
   printf(fmt,"ARGLIMIT",x);
   x=(-(x-1.00000000000000));  printf(fmt,"NEGARGLIMIT",x);
   x = DBL_EPSILON;  x=x+x; printf(fmt,"MACHINE_PRECISION",x);
   x = 1.0000000000000000000/x;  printf(fmt,"INVERSE_MACHINE_PRECISION",x);
   x = log(MAX_REAL_TYPE)*approx_one; printf(fmt,"LN_MAX_REAL_TYPE",x);
   x = sqrt(MAX_REAL_TYPE)*approx_one; printf(fmt,"SQRT_MAX_REAL_TYPE",x);
   pi = 2.00000000000000 * atan2(1.0000000000000,0.0000000000000);
   printf(fmt,"PI",pi);
   return(1);
}
