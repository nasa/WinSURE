#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>


/*
===============================================================================

        Low level combinarorial functions required by ASSIST and SURE 
        compiler programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)

===============================================================================
*/

#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"

#ifdef PASCAL_SURE
#include "perrdefs.h"
#else
#include "errdefs.h"
#endif

#if !defined(OMIT_MATH_CHECKS)
#include "safeeval.h"
#endif

#include "fact_ext.h"
#include "prs_math.h"

/*=========================================================================*/
int_type ifact(int_type n)
{
   register int_type ans;
   register int_type i,ie;
   /*------------------------------------------------------------------*/
   if (n<0) IERR(E_NEGVALUE,n," (FACT) n < ",0);
   else if (n<2) return(1);
   ans = 2;
   for (i=3,ie=n;i<=ie;++i) ans = impy(ans,i);
   /*------------------------------------------------------------------*/
   return(ans);
}
/*=========================================================================*/
int_type icomb(int_type n,int_type k)
{
   register int_type ans;
   register int_type i,ie,ios;
   real_type rans;
   /*------------------------------------------------------------------*/
   if ((n<0)||(k<0)) IERR(E_NEGVALUE,n," (COMB) n < ",0);
   else if (k>n) IERR(E_KVSN,k," (COMB) k < n = ",n);
   else if ((n<2)||(n==k)||(k==0)) return(1);
   i = n;
   rans = (real_type) i;
   ie = (((2*k) > n) ? (i-k) : k);
   ios = (i-ie);
#ifdef CDC
   IERR(E_INTEGEROVF,n," COMB(n,k) ",k);
#endif
   for (--i;i>ie;--i) rans = rmpy(rans,((real_type) i));
   for (i=ios;i>1;--i) rans = rdvd(rans,((real_type) i));
   /*------------------------------------------------------------------*/
   ans = (int_type) (rans+0.50000000000);
   return(ans);
}
/*=========================================================================*/
int_type iperm(int_type n,int_type k)
{
   register int_type ans;
   register int_type i,ie;
   /*------------------------------------------------------------------*/
   if ((n<0)||(k<0)) IERR(E_NEGVALUE,n," (PERM) n < ",0);
   else if (k>n) IERR(E_KVSN,k," (PERM) k < n = ",n);
   else if ((n<2)||(k==0)) return(1);
   ans = i = n;
   ie = i - k;
   for (--i;i>ie;--i) ans = impy(ans,i);
   /*------------------------------------------------------------------*/
   return(ans);
}
/*=========================================================================*/

