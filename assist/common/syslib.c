/*
===============================================================================

        System Dependent functions required by ASSIST and SURE compiler
        programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)
             10/01   RWB changed AMOUNT

===============================================================================
*/

/* define TIME_REQD so that "cm_sys.h" will know to pull in header files */
#define TIME_REQD

#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"
#include "cm_types.h"
#include "cm_vars.h"
#include "pass.h"

#include "sys_ext.h"

#ifdef IMPRECISE_CLOCK
   #include <sys/time.h>
   #include <sys/resource.h>
   #define AMOUNT 1000000       // RWB changed it from 100 to 1000000
#else
  #define AMOUNT CLK_TCK
#endif

static clock_t tick0,tick9,tickdiff;
static char fmt_lf_00s[14];

/*===========================================================================*/
void init_syslib(void)
{
   /*------------------------------------------------------------------*/
   inputfile=outputfile=modfile=listfile=objectfile=xreffile=
             exefile=scrfile=bugfile=((FILE *) NULL);
   bugfile=stderr;
   outputfile=stdout;
   errorfile=stderr;
   init_elapsed_time();
   sprintf(fmt_lf_00s,"%%2.2%s",REAL_TYPE_FORMAT);
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void re_init_syslib(void)
{
   /*------------------------------------------------------------------*/
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void init_elapsed_time()
{
   /*------------------------------------------------------------------*/
   tick0=my_clock();
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void shew_elps(FILE *fp,char *msg)
{
   real_type quo;
   /*------------------------------------------------------------------*/
   if (fp==((FILE *) NULL)) return;
   tickdiff=tick9-tick0;

   quo = (real_type) AMOUNT;
   quo = ((real_type) tickdiff)/quo;
   fprintf(fp,"%s TIME = ",msg);
   fprintf(fp,fmt_lf_00s,quo);
   fprintf(fp," sec.\n");
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void show_elapsed_processing_time()
{
   /*------------------------------------------------------------------*/
   tick9=my_clock();
   shew_elps(outputfile,"PARSING");
   shew_elps(listfile,"PARSING");
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void show_elapsed_generation_time()
{
   /*------------------------------------------------------------------*/
   tick9=my_clock();
   shew_elps(outputfile,"RULE GENERATION");
   shew_elps(listfile,"RULE GENERATION");
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void show_aggr_knt
     (FILE *fp,long knt,long s0,long ns,char *kind,char *pre,char *post)
{
   /*------------------------------------------------------------------*/
   if ((fp != ((FILE *) NULL)) && (ns>0)){
      fprintf(fp,"%s%d %s AGGREGATED INTO STATE%s %d",
              pre,(int) knt,kind,((ns>1) ? "S" : ""),(int) s0);
      if (ns>1) fprintf(fp," - %d",(int) (s0+ns-1));
      fprintf(fp,"%s\n",post);}
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
void make_date_and_time()
{
   time_t nowtim;
   struct tm *tmrec;
   Boolean is_pm;
   int hour;
   /*-------------------------------------------------------*/
   nowtim=time(NULL);
   tmrec=localtime(&nowtim);
   sprintf(date_string,"%d/%02d/%02d",
           (int) (1+(*tmrec).tm_mon),
           (int) (*tmrec).tm_mday,
           (int) ((*tmrec).tm_year % 100));
   is_pm=(Boolean) ((*tmrec).tm_hour>=12);
   hour=(*tmrec).tm_hour;
   if (is_pm && (hour>12)) hour -= 12;
   if ((!is_pm) && (hour<1)) hour += 12;
   sprintf(time_string,"%d:%02d:%02d %s",
           hour,(int) (*tmrec).tm_min,(int) (*tmrec).tm_sec,
           (is_pm ? "p.m." : "a.m."));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void g_malloc_abort_if_error(void *ppp)
{
   if (ppp == ((void *) NULL)){
       fprintf(stderr,"[ERROR] Aborting program due to insufficient memory\n");
       exit(1);}
}
/*===========================================================================*/
#if (!defined(MSDOS))
void *g_malloc(int nby)
{
   void *ans;
   /*------------------------------------------------------------------*/
   ans = malloc(nby);
   if (ans != ((void *) NULL)){
      sys_memset((char *) ans,0,nby);}
   /*------------------------------------------------------------------*/
   return(ans);
}
#endif
/*===========================================================================*/
#if (!defined(MSDOS))
void *g_realloc(void *ppp,int nby,int onby)
{
   void *ans;
   char *sss;
   /*------------------------------------------------------------------*/
   ans = realloc(ppp,nby);
   if (ans != ((void *) NULL)){
      sss = (char *) ans;
      sss += onby;
      sys_memset(sss,0,nby-onby);}
   /*------------------------------------------------------------------*/
   return(ans);
}
#endif
/*======================================================================= RWB
#ifdef IMPRECISE_CLOCK
extern void getrusage(int,struct rusage *);
long my_clock()
{
   struct rusage ru;
   register long hunds;

   getrusage(RUSAGE_SELF,&ru);
   hunds = (ru.ru_utime.tv_sec+ru.ru_stime.tv_sec)*100L +
           (ru.ru_utime.tv_usec+ru.ru_stime.tv_usec)/10000L;

   return(hunds);
}
#endif
RWB ==========================================================================*/
