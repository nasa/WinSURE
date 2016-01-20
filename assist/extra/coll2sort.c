/*
 *
 *   ------------------------------------------------------------
 *   Sorts a collectively modelled ".mod" file from the standard
 *   input and writes it to the standard output.
 *   ------------------------------------------------------------
 *
 *   Author:  David P. Boerschlein
 *
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#include "ind_vs_coll.h"

/*??????????????????*/ FILE *dumpfile = NULL;

extern int tto_cmp(void *,void *);
void sort_transitions(void);
void process_transition(char *);
extern void print_transitions(FILE *);
extern void parse(FILE *,FILE *);
extern void init_stuff(void);
extern void re_init_stuff(Boolean);
extern int main(int,char **);

int tto_cmp(void *xtto1,void *xtto2)
{
   int ord;
   int srceqv1,desteqv1;
   int srceqv2,desteqv2;
   transition_info_type *tto1;
   transition_info_type *tto2;
   tto1 = (transition_info_type *) xtto1;
   tto2 = (transition_info_type *) xtto2;
   srceqv1 = (*tto1).start_statenum;
   desteqv1 = (*tto1).dest_statenum;
   srceqv2 = (*tto2).start_statenum;
   desteqv2 = (*tto2).dest_statenum;
   ord = tto_cmp_eqvs(srceqv1,desteqv1,srceqv2,desteqv2,tto1,tto2);
   return(ord);
}

void sort_transitions()
{
   qsort((char *) &trantos[0],tranto_knt,sizeof(transition_info_type),tto_cmp);
}

void process_transition(char *line)
{
   transition_info_type *tto;
   char *ppp;
   tto = &trantos[tranto_knt];
   ++tranto_knt;
   if (tranto_knt >= MAX_TRANTO_KNT){
       fprintf(stderr,"Too many transitions ... Processing aborted ...\n");
       exit(1);}
   ppp = line;
   ppp = strip_state(ppp,&(*tto).start_statenum);
   if ((*ppp)==',') ++ppp;
   ppp = strip_state(ppp,&(*tto).dest_statenum);
   while ((isspace(*ppp)) && ((*ppp)!= '\0')) ++ppp;
   if ((*ppp)==EQUALS) ++ppp;
   while ((isspace(*ppp)) && ((*ppp)!= '\0')) ++ppp;
   (*tto).rate = malloc_copy(ppp);
}

void print_transitions(FILE *fpout)
{
   FILE *fp;
   int ixtto;
   int srcnum,destnum;
   transition_info_type *tto;
   transition_info_type *ttoovf;
   tto = &trantos[0];
   ttoovf = &trantos[tranto_knt];
   for (ixtto=0;tto < ttoovf;++ixtto,++tto){
        srcnum = (*tto).start_statenum;
        destnum = (*tto).dest_statenum;
        if (states[srcnum].coll_comments){
            fp = fpout;
            if ((fp != ((FILE *) NULL)) && (states[srcnum].coll_comments)){
                fprintf(fp,"%5d(* %s *),%5d(* %s *) = ",
                           (int) srcnum,
                           states[srcnum].coll_comments,
                           (int) destnum,
                           states[destnum].coll_comments);
                out_rate(fp,(*tto).rate,1,1);
                fprintf(fp,"\n");}}}
}

void parse(FILE *fpin,FILE *fpout)
{
   int nlines;
   nlines=parse_file(fpin,fpout,process_transition);
   fprintf(stderr,"%4d transitions parsed\n",nlines);
   fprintf(stderr,"Down-numbering .... (transitions)\n");
   sort_transitions();
   fprintf(stderr,"Combining .... (transitions)\n");
   print_transitions(fpout);
   fprintf(stderr,"Done printing....\n");
}

void init_stuff()
{
}

void re_init_stuff(Boolean freeit)
{
   init_ind_vs_coll(freeit);
}

int main(int argc,char **argv)
{
   fprintf(stderr,"[coll2sort 1.1]\n");
   parse_ind_vs_coll_argv(argc,argv);
   init_stuff();
   re_init_stuff(FALSE);
   parse(stdin,stdout);
   return(0);
}
