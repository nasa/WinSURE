#include "main_def.h"
#include "cm_sys.h"
#include "commonio.h"
#include "fact_ext.h"

static Boolean in_error=FALSE;

extern int main(int,char **);
extern void enk_commas(int_type,int,char *);

real_type real_eval_err(short ee,real_type a,char *s,real_type b)
{
   in_error=TRUE;
   return((real_type) 0.00000000000000);
}

int_type int_eval_err(short ee,int_type a,char *s,int_type b)
{
   in_error=TRUE;
   return((int_type) 0);
}

void enk_commas(int_type val,int nch,char *s)
{
   int_type units,thous,mills,bills;
   char lenk_units[6],lenk_thous[6],lenk_mills[6],lenk_bills[6];
   static char fmt0[] = "%4d";
   static char fmt1[] = ",%03d";
   static char fmtb[] = "    ";
   char lenk_full[20];
   char *fmt;
   units = val % 1000;
   thous = (val/1000) % 1000;
   mills = (val/1000000) % 1000;
   bills = (val/1000000000) % 1000;
   fmt=fmt0;
   if (bills>0) {sprintf(lenk_bills,fmt,(int) bills); fmt=fmt1;}
   else strcpy(lenk_bills,fmtb);
   if ((mills>0)||(fmt==fmt1)) {sprintf(lenk_mills,fmt,(int) mills); fmt=fmt1;}
   else strcpy(lenk_mills,fmtb);
   if ((thous>0)||(fmt==fmt1)) {sprintf(lenk_thous,fmt,(int) thous); fmt=fmt1;}
   else strcpy(lenk_thous,fmtb);
   if ((units>0)||(fmt==fmt1)) {sprintf(lenk_units,fmt,(int) units); fmt=fmt1;}
   else strcpy(lenk_units,fmtb);
   strcpy(lenk_full,lenk_bills);
   strcat(lenk_full,lenk_mills);
   strcat(lenk_full,lenk_thous);
   strcat(lenk_full,lenk_units);
   strcpy(s,lenk_full+16-nch);
}

#define DO_ENK(what,where) \
        in_error=FALSE; \
        enk_commas(what,15,where); \
        if (in_error) strcpy(where,"        <error>")

int main(int argc,char **argv)
{
   int_type n;
   char lenk_fact[20];
   char lenk_perma[20];   char lenk_permb[20];
   char lenk_permc[20];   char lenk_permd[20];
   char lenk_comba[20];   char lenk_combb[20];
   char lenk_combc[20];   char lenk_combd[20];
   /*==================================================================*/
   if (argc>1) freopen(argv[1],"w",stdout);
   else freopen("facttest.out","w",stdout);
   /*==================================================================*/
   printf(" n        n!          comb(n,n)     comb(n,n-1)    comb(n,n-2)    comb(n,n-3)\n");
   printf("---  -------------  -------------  -------------  -------------  -------------\n");
   for (n=0;n<21;++n){
      DO_ENK(ifact(n),lenk_fact);
      DO_ENK(icomb(n,n),lenk_comba);
      DO_ENK(icomb(n,(int_type) (n-1)),lenk_combb);
      DO_ENK(icomb(n,(int_type) (n-2)),lenk_combc);
      DO_ENK(icomb(n,(int_type) (n-3)),lenk_combd);
      printf("%3d%s%s%s%s%s\n",
             (int) n,lenk_fact,lenk_comba,lenk_combb,lenk_combc,lenk_combd);}
   /*==================================================================*/
   printf(" n        n!          comb(n,0)      comb(n,1)      comb(n,2)      comb(n,3)\n");
   printf("---  -------------  -------------  -------------  -------------  -------------\n");
   for (n=0;n<21;++n){
      DO_ENK(ifact(n),lenk_fact);
      DO_ENK(icomb(n,(int_type) 0),lenk_comba);
      DO_ENK(icomb(n,(int_type) 1),lenk_combb);
      DO_ENK(icomb(n,(int_type) 2),lenk_combc);
      DO_ENK(icomb(n,(int_type) 3),lenk_combd);
      printf("%3d%s%s%s%s%s\n",
             (int) n,lenk_fact,lenk_comba,lenk_combb,lenk_combc,lenk_combd);}
   /*==================================================================*/
   printf(" n        n!          perm(n,n)     perm(n,n-1)    perm(n,n-2)    perm(n,n-3)\n");
   printf("---  -------------  -------------  -------------  -------------  -------------\n");
   for (n=0;n<21;++n){
      DO_ENK(ifact(n),lenk_fact);
      DO_ENK(iperm(n,n),lenk_perma);
      DO_ENK(iperm(n,(int_type) (n-1)),lenk_permb);
      DO_ENK(iperm(n,(int_type) (n-2)),lenk_permc);
      DO_ENK(iperm(n,(int_type) (n-3)),lenk_permd);
      printf("%3d%s%s%s%s%s\n",
             (int) n,lenk_fact,lenk_perma,lenk_permb,lenk_permc,lenk_permd);}
   /*==================================================================*/
   printf(" n        n!          perm(n,0)      perm(n,1)      perm(n,2)      perm(n,3)\n");
   printf("---  -------------  -------------  -------------  -------------  -------------\n");
   for (n=0;n<21;++n){
      DO_ENK(ifact(n),lenk_fact);
      DO_ENK(iperm(n,(int_type) 0),lenk_perma);
      DO_ENK(iperm(n,(int_type) 1),lenk_permb);
      DO_ENK(iperm(n,(int_type) 2),lenk_permc);
      DO_ENK(iperm(n,(int_type) 3),lenk_permd);
      printf("%3d%s%s%s%s%s\n",
             (int) n,lenk_fact,lenk_perma,lenk_permb,lenk_permc,lenk_permd);}
   /*==================================================================*/
}

