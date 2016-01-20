#include "main_def.h"
#include "cm_sys.h"
#include "cm_defs.h"

#include "gammalim.h"
#include "gamma.h"

extern int main(int,char **);
int main(int argc,char **argv)
{
   double ans;
   static double p[] = 
      {
          0.3, 0.4, 0.7, 1.0, 1.3, 2.1, 2.2, 
          4.5, 12.1, 50., .71e15
      };
   static double pc[] = 
      {
          0.9, 1.5, 1.92, 2.0, 2.2, 2.95, 3.5, 
          11.87, 12.6, 167.51, 169.3
      };
   int i,ierr;
   printf("  p[i]          lngamma(p[i],&ans)       ierr\n");
   printf("--------     ------------------------    ----\n");
   for (i=0;i<11;++i){
      ierr=lngamma(p[i],&ans);
      printf("%8.3lf    %25.14le    %d\n",p[i],ans,ierr);}
   printf(" pc[i]           cgamma(p[i],&ans)       ierr\n");
   printf("--------     ------------------------    ----\n");
   for (i=0;i<11;++i){
      ierr=cgamma(pc[i],&ans);
      printf("%8.3lf    %25.14le    %d\n",pc[i],ans,ierr);}
   return(1);
}
