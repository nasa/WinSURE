#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define IND_VS_COLL_ROUTINES
#include "ind_vs_coll.h"

#ifdef UNIX
extern int stricmp(char *,char *);
extern void fflush(FILE *);
#endif

#ifdef LINUX
int stricmp(const char *s1, const char *s2)
{
     char ch1, ch2;
     int iret;

     ch1 = *s1; ch2 = *s2; iret = 0;
     while (((ch1 = *s1++) != '\0') & ((ch2 = *s2++) != '\0'))   { 
         ch1 = tolower(ch1); ch2 = tolower(ch2);
         if (ch1 < ch2) {iret = -1; goto L;}
         if (ch1 > ch2) {iret = +1; goto L;}
	   }; 
     if ( (ch1 == '\0') & (ch2 == '\0')) goto L;
     if (ch1 == '\0') iret = +1;
     if (ch2 == '\0') iret = -1;
   L:
     return iret;
 }
#endif

extern FILE *dumpfile;

void abort_program()
{
   if (dumpfile) fflush(dumpfile);
   fflush(stdout);
   *((char *) 0) = '\0';
}

void slow_memmove(char *dest,char *src,int nch)
{
   register char *psrc;
   register char *pdest,*povf;
   /*-------------------------------------------------------*/
   if (dest>src){
      for (pdest=dest+nch-1,psrc=src+nch-1;pdest>=dest;--pdest,--psrc){
         *pdest = *psrc;}}
   else{
      for (pdest=dest,psrc=src,povf=pdest+nch;pdest<povf;++pdest,++psrc){
         *pdest = *psrc;}}
   /*-------------------------------------------------------*/
}

char *malloc_copy(char *sss)
{
   char *ans;
   ans = malloc(1+strlen(sss));
   if (ans) strcpy(ans,sss);
   else{
       fprintf(stderr,"Attempt to allocate more memory failed.\n");
       exit(1);}
   return(ans);
}

char *strip_number(char *pppin,int *number)
{
   char *ppp;
   char *qqq;
   char enknum[33];
   for (ppp=pppin;isspace((*ppp))&&((*ppp)!='\0');++ppp);
   for (qqq=enknum;isdigit((*ppp));++ppp,++qqq) *qqq = *ppp;
   *qqq='\0';
   *number = (int) atol(enknum);
   return(ppp);
}

char *strip_state_comments(char *pppin,char *state,int snum)
{
#define SkipSpaces(ppp) \
        while (isspace(*ppp)) ++ppp
#define CommentTerminator(ppp) \
        ((isspace(*(ppp)))||((*(ppp))==STAR))
   char *ppp;
   char *qqq;
   char tail[82];
   *state = '\0';
   ppp = pppin;
   if ((*ppp)==OPENPAREN){
       if ((*((++ppp)))==STAR){
           ++ppp; /* Skip the STAR */
           SkipSpaces(ppp);
           for (qqq=state;!CommentTerminator(ppp);++ppp,++qqq) *qqq = *ppp;
           *qqq = '\0';
           SkipSpaces(ppp);
           *tail = '\0';
           if (stricmp(state,"TRIM.D")==0) strcpy(tail,state);
           if (stricmp(state,"TRIM")==0) strcpy(tail,state);
           if (stricmp(state,"UNKNOWN")==0) strcpy(tail,state);
           if (isalnum(*ppp)){
               for (qqq=tail;!CommentTerminator(ppp);++ppp,++qqq)
                    *qqq = *ppp;
               *qqq = '\0';
               SkipSpaces(ppp);}
           if ((*tail)!='\0'){
               if (stricmp(tail,"TRIM.D")==0) sprintf(state,"%s.%d",tail,snum);
               if (stricmp(tail,"TRIM")==0) sprintf(state,"%s.%d",tail,snum);
               if (stricmp(tail,"UNKNOWN")==0) sprintf(state,"%s.%d",tail,snum);
               if (stricmp(tail,"DEATH")==0) sprintf(state,"%s.%d",tail,snum);
               if (stricmp(tail,"PRUNE")==0) sprintf(state,"%s.%d",tail,snum);}
           if (*ppp==STAR) ++ppp;
           if (*ppp==CLOSEPAREN) ++ppp;}}
   return(ppp);
#undef CommentTerminator
#undef SkipSpaces
}

char *strip_state(char *pppin,int *statenum)
{
   char comments[1024];
   state_info_type *state;
   char *ppp;
   ppp = pppin;
   ppp = strip_number(ppp,statenum);
   if ((*statenum) >= state_knt){
       state_knt = 1+(*statenum);
       if (state_knt > MAX_STATE_KNT){
           fprintf(stderr,"Too many states: %d\n",(int) *statenum);
           abort_program();}}
   state = &states[*statenum];
   ppp = strip_state_comments(ppp,comments,*statenum);
   if ((*state).indiv_comments){
       if (strcmp(comments,(*state).indiv_comments) != 0){
           if (debug && dumpfile){
               fprintf(dumpfile,"\n%s\n",pppin);
               fprintf(dumpfile,"State indiv mismatch \"%s\" vs \"%s\"\n",
                                comments,(*state).indiv_comments);}
           fprintf(stderr,"State indiv mismatch \"%s\" vs \"%s\"\n",
                          comments,(*state).indiv_comments);
           abort_program();}}
   else{
       if (debug && dumpfile)
           fprintf(dumpfile,"adding state[%d] = \"%s\"\n",*statenum,comments);
       (*state).indiv_comments = malloc_copy(comments);}
   cvt_to_coll((*state).indiv_comments,comments);
   if ((*state).coll_comments){
       if (strcmp(comments,(*state).coll_comments) != 0){
           if (debug && dumpfile){
               fprintf(dumpfile,"\n%s\n",pppin);
               fprintf(dumpfile,"State coll mismatch \"%s\" vs \"%s\"\n",
                                comments,(*state).coll_comments);}
           fprintf(stderr,"State coll mismatch \"%s\" vs \"%s\"\n",
                          comments,(*state).coll_comments);
           abort_program();}}
   else
       (*state).coll_comments = malloc_copy(comments);
   return(ppp);
}

char *cvt_1_to_coll(char *src,char *dest)
{
   char *sss;
   char *ddd;
   int knt;
   if ((*src)!=OPENPAREN){
       fprintf(stderr,"Cannot parse model file input.\n");
       fprintf(stderr,"Be sure that COMMENT ON is set when ");
       fprintf(stderr,"pre-processing with assist.\n");
       exit(1);}
   sss = src;
   ddd = dest;
   knt = 0;
   for (++sss;(*sss)!=CLOSEPAREN;){
        if ((*sss)=='T') ++knt;
        while (((*sss)!=',')&&((*sss)!=CLOSEPAREN)) ++sss;
        if ((*sss)==',') ++sss;}
   sprintf(dest,"%d",(int) knt);
   ++sss;
   return(sss);
}

char *cvt_to_coll(char *src,char *dest)
{
   char *sss;
   char *ddd;
   sss = src;
   ddd = dest;
   while ((*sss)!='\0'){
          while (((*sss)!='\0')&&((*sss)!=OPENPAREN)) *ddd++ = *sss++;
          *ddd = '\0';
          if ((*sss)!='\0'){
              sss = cvt_1_to_coll(sss,ddd);
              ddd += strlen(ddd);}}
   return(sss);
}

void RemoveTrailingNewlineWhitespace(char *msg)
{
   char *ppp;
   if ((*msg)=='\0') return;
   ppp = msg+strlen(msg)-1;
   if ((*ppp)=='\n') *ppp-- = '\0';
   for (;(ppp>=msg) && (isspace(*ppp));--ppp) *ppp='\0';
}

Boolean text_is_blank(char *msg)
{
   char *ppp;
   for (ppp=msg;*ppp;++ppp) if (!isspace(*ppp)) return(FALSE);
   return(TRUE);
}


int parse_file(FILE *fpin,FILE *fpout,any_process_routine process_transition)
{
   char line[4194];
   char templine[1026];
   char *temptr;
   char *eee;
   int nnn,ixline;
   Boolean more;
   char buff[33];
#ifdef BUGGY
   int iii,jjj,ix;
#endif
   fprintf(stderr,"Parsing .... (declarations)\n");
   while (fgets(line,1024,fpin)){
          RemoveTrailingNewlineWhitespace(line);
          fprintf(fpout,"%s\n",line);
          if (strstr(line,"PRUNESTATE")) goto done_first;}
done_first:
   fprintf(stderr,"Parsing .... (transitions)\n");
   strcpy(buff,"0 (* UNKNOWN *),");
   strip_state(buff,&nnn);
   more = TRUE;
   for (ixline=0;more && fgets(line,1024,fpin);++ixline){
        if ((ixline > 0) && ((ixline % 1000) == 0))
            fprintf(stderr,"%4d transitions parsed\n",ixline);
        /*-------------------------  read a line */
        if (((*line)==OPENPAREN)&&((*(line+1))=STAR)) *line='\0';
#ifdef OBSOLETE
        no_comments(line);
#endif
        RemoveTrailingNewlineWhitespace(line);
        if (strlen(line)>0){
            /*----------------  combine lines when continued */
            nnn = strlen(line)-1;
            while ((line[nnn] != ';')&&(!feof(fpin))){
                   fgets(templine,1024,fpin);
#ifdef OBSOLETE
                   no_comments(templine);
#endif
                   RemoveTrailingNewlineWhitespace(templine);
                   temptr = templine;
                   while ((*temptr) && (isspace(*temptr))) ++temptr;
                   eee = line + strlen(line) - 1;
                   if ((isalnum(*temptr))&&(isalnum(*eee))) strcat(line," ");
                   strcat(line,temptr);
                   nnn = strlen(line)-1;}
            line[nnn] = '\0';
            /*----------------  process transition */
            if (text_is_blank(line)) more=FALSE;
            else process_transition(line);}}
   fprintf(fpout,"\n");
   return(ixline);
}


void parse_ind_vs_coll_argv(int argc,char **argv)
{
   int iarg;
   combine_products = debug = FALSE;
   aux_trn_knt = 0;
   for (iarg=1;iarg<argc;++iarg){
       if (IS_ARGV_OPTION(argv[iarg])){
           if (stricmp(&argv[iarg][1],"trn")==0) aux_trn_file(argv[++iarg]);
           else if (stricmp(&argv[iarg][1],"debug")==0) debug = TRUE;
           else if (stricmp(&argv[iarg][1],"cp")==0) combine_products = TRUE;}}
}

void init_ind_vs_coll(Boolean freeit)
{
   int iii;
   tranto_knt = 0;
   state_knt = 0;
   for (iii=0;iii<MAX_STATE_KNT;++iii){
        if (freeit){
            FREEIF(states[iii].indiv_comments);
            FREEIF(states[iii].coll_comments);}
        states[iii].indiv_comments = (char *) NULL;
        states[iii].coll_comments = (char *) NULL;}
   for (iii=0;iii<MAX_TRANTO_KNT;++iii){
        if (freeit){
            FREEIF(trantos[iii].rate);}
        trantos[iii].start_statenum = trantos[iii].dest_statenum = (-99);
        trantos[iii].tranto_repeat = 1;
        trantos[iii].rate = (char *) NULL;}
}

void translate_if_maybe(char *sss,char *look,char *sub)
{
   char *ppp;
   char *qqq;
   char looklast;
   int nchlook,nchsub;
   Boolean doit;
   looklast = (*(look+strlen(look)-1));
   if (ppp=strstr(sss,look)){
       qqq = ppp+strlen(look);
       doit = (Boolean)
          (
             ((ppp==sss)||(!(isalnum(*(ppp-1))))) &&
             (((*qqq)=='\0')||(!isalnum(*qqq))||(looklast=='*'))
          );
       if (doit){
           nchlook = strlen(look);
           nchsub = strlen(sub);
           if (nchsub != nchlook) 
               slow_memmove(ppp+nchsub,ppp+nchlook,1+strlen(ppp));
           if (nchsub > 0) memcpy(ppp,sub,nchsub);}}
}

void translate(char *sss,char *look,char *sub)
{
   char *ppp;
   int nchlook,nchsub;
   if (ppp=strstr(sss,look)){
       nchlook = strlen(look);
       nchsub = strlen(sub);
       if (nchsub != nchlook) 
           slow_memmove(ppp+nchsub,ppp+nchlook,1+strlen(ppp));
       if (nchsub > 0) memcpy(ppp,sub,nchsub);}
}

void combine_1_difference(char *rate,int val,int minus)
{
   char enk[32],sub[32];
   int kkk;
   sprintf(enk,"(%d-%d)",val,minus);
   *sub='\0';
   kkk = val-minus;
   sprintf(sub,"%d",kkk);
   translate(rate,enk,sub);
}

void combine_1_sum(char *rate,int val,int plus)
{
   char enk[32],sub[32];
   int kkk;
   sprintf(enk,"(%d+%d)",val,plus);
   *sub='\0';
   kkk = val+plus;
   sprintf(sub,"%d",kkk);
   translate(rate,enk,sub);
}

void combine_1_left_factor(char *rate,int valfac,char *fromfac,int newfac)
{
   char aaa[32],bbb[32];
   int iii,jjj,nnn;
   Boolean doit;
   doit = TRUE;
   if (((*fromfac)!='\0') && (!strstr(rate,fromfac))) doit=FALSE;
   if (doit){
       for (iii=0;iii<COMPARISON_LOOP_MAX;++iii){
            jjj = 1 + iii;
            sprintf(aaa,"(%d)%s*",(int) (valfac*jjj),fromfac);
            nnn = (int) (newfac*jjj);
            bbb[0] = '\0';
            if (nnn != 1) sprintf(bbb,"%d*",nnn);
            translate(rate,aaa,bbb);
            sprintf(aaa,"%d%s*",(int) (valfac*jjj),fromfac);
            translate_if_maybe(rate,aaa,bbb);}}
}

void combine_1_right_factor(char *rate,char *fromfac,int valfac,int newfac)
{
   char aaa[32],bbb[32];
   int iii,jjj,nnn;
   Boolean doit;
   doit = TRUE;
   if (((*fromfac)!='\0') && (!strstr(rate,fromfac))) doit=FALSE;
   if (doit){
       for (iii=0;iii<COMPARISON_LOOP_MAX;++iii){
            jjj = 1 + iii;
            sprintf(aaa,"%s(%d)*",fromfac,(int) (valfac*jjj));
            nnn = (int) (newfac*jjj);
            bbb[0] = '\0';
            if (nnn != 1) sprintf(bbb,"%d*",nnn);
            translate(rate,aaa,bbb);
            sprintf(aaa,"%s%d*",fromfac,(int) (valfac*jjj));
            translate(rate,aaa,bbb);
            sprintf(aaa,"%s%d*",fromfac,(int) (valfac*jjj));
            translate_if_maybe(rate,aaa,bbb);}}
}

void combine_val_2_front(char *rate)
{
   char aaa[32];
   char *rrrr;
   char *ppp;
   char *qqq;
   int iii,nnn,kkk;
   rrrr = rate;
   if (strncmp(rate,"FAST ",5) == 0) rrrr = rate + 5;
   for (iii=0;iii<8;++iii){
        sprintf(aaa,"*%d*",iii);
        for (kkk=0;(kkk<8)&&(ppp=strstr(rrrr,aaa));++kkk){
             qqq = ppp+(nnn=strlen(aaa)-1);
             slow_memmove(ppp,qqq,1+strlen(qqq));
             slow_memmove(rrrr+nnn,rrrr,1+strlen(rrrr));
             memcpy(rrrr,aaa+1,nnn);}}
}

extern void Qcombine(char *);
void Qcombine(char *rate)
{
   int iii,jjj;
   /**/
   for (iii=0;iii<aux_trn_knt;++iii){
        translate(rate,aux_from[iii],aux_to[iii]);}
   /**/
   for (iii=0;iii<=12;++iii){
        for (jjj=0;jjj<=12;++jjj){
             combine_1_sum(rate,iii,jjj);
             if (iii>=jjj)
                 combine_1_difference(rate,iii,jjj);}}
   /**/
   combine_val_2_front(rate);
   /**/
   translate(rate,"(1/1)","1");
   translate(rate,"(2/2)","1");
   translate(rate,"(3/3)","1");
   /**/
   combine_1_left_factor(rate,4,"*(0.25)",1);
   combine_1_left_factor(rate,4,"*0.25",1);
   /**/
   combine_1_left_factor(rate,4,"*(1.0/4.0)",1);
   /**/
   combine_1_left_factor(rate,2,"*(0.50)",1);
   combine_1_left_factor(rate,2,"*0.50",1);
   combine_1_left_factor(rate,2,"*(0.5)",1);
   combine_1_left_factor(rate,2,"*0.5",1);
   /**/
   combine_1_left_factor(rate,2,"*(1.0/2.0)",1);
   combine_1_left_factor(rate,2,"*(1.0/(2))",1);
   combine_1_left_factor(rate,2,"*(1.0/2)",1);
   combine_1_left_factor(rate,2,"*(1/2.0)",1);
   combine_1_left_factor(rate,2,"*(1/(2))",1);
   combine_1_left_factor(rate,2,"*(1/2)",1);
   /**/
   combine_1_left_factor(rate,3,"*(1.0/3.0)",1);
   combine_1_left_factor(rate,3,"*(2.0/3.0)",2);
   combine_1_left_factor(rate,3,"*(1.0/(3))",1);
   combine_1_left_factor(rate,3,"*(2.0/(3))",2);
   combine_1_left_factor(rate,3,"*(1.0/3)",1);
   combine_1_left_factor(rate,3,"*(2.0/3)",2);
   combine_1_left_factor(rate,3,"*(1/3.0)",1);
   combine_1_left_factor(rate,3,"*(2/3.0)",2);
   combine_1_left_factor(rate,3,"*(1/(3))",1);
   combine_1_left_factor(rate,3,"*(2/(3))",2);
   combine_1_left_factor(rate,3,"*(1/3)",1);
   combine_1_left_factor(rate,3,"*(2/3)",2);
   /**/
   combine_1_right_factor(rate,"(0.25)*",4,1);
   combine_1_right_factor(rate,"0.25*",4,1);
   /**/
   combine_1_right_factor(rate,"(1.0/4.0)*",4,1);
   /**/
   combine_1_right_factor(rate,"(0.50)*",2,1);
   combine_1_right_factor(rate,"0.50*",2,1);
   combine_1_right_factor(rate,"(0.5)*",2,1);
   combine_1_right_factor(rate,"0.5*",2,1);
   /**/
   combine_1_right_factor(rate,"(1.0/2.0)*",2,1);
   combine_1_right_factor(rate,"(1.0/(2))*",2,1);
   combine_1_right_factor(rate,"(1.0/2)*",2,1);
   combine_1_right_factor(rate,"(1/2.0)*",2,1);
   combine_1_right_factor(rate,"(1/(2))*",2,1);
   combine_1_right_factor(rate,"(1/2)*",2,1);
   /**/
   combine_1_right_factor(rate,"(1.0/3.0)*",3,1);
   combine_1_right_factor(rate,"(2.0/3.0)*",3,2);
   combine_1_right_factor(rate,"(1.0/(3))*",3,1);
   combine_1_right_factor(rate,"(2.0/(3))*",3,2);
   combine_1_right_factor(rate,"(1.0/3)*",3,1);
   combine_1_right_factor(rate,"(2.0/3)*",3,2);
   combine_1_right_factor(rate,"(1/3.0)*",3,1);
   combine_1_right_factor(rate,"(2/3.0)*",3,2);
   combine_1_right_factor(rate,"(1/(3))*",3,1);
   combine_1_right_factor(rate,"(2/(3))*",3,2);
   combine_1_right_factor(rate,"(1/3)*",3,1);
   combine_1_right_factor(rate,"(2/3)*",3,2);
   /**/
   combine_1_left_factor(rate,1,"",1);
   /**/
   combine_1_right_factor(rate,"",1,1);
   /**/
   translate(rate,"(2.0/3.0)*2*","4*(1.0/3.0)*");
   translate(rate,"(1.0/3.0)*2*","2*(1.0/3.0)*");
   translate(rate,"*2*(2.0/3.0)","4*(1.0/3.0)*");
   translate(rate,"*2*(1.0/3.0)","2*(1.0/3.0)*");
   /**/
   translate(rate,"(1)*","");
   translate(rate,"*(1)","");
   translate(rate," 1*"," ");
   /**/
   translate(rate," 2*(0.25)*"," (0.50)*");
   if (strncmp(rate,"2*(0.25)*",9)==0){
       slow_memmove(rate+7,rate+9,1+strlen(rate+9));
       memcpy(rate,"(0.50)*",7);}
   /**/
   if (strncmp(rate,"1*",2)==0)
       slow_memmove(rate,rate+2,1+strlen(rate+2));
}

void combine(char *rate)
{
   Qcombine(rate);
   Qcombine(rate);
}

void aux_trn_file(char *fname)
{
   FILE *fp;
   char line[88];
   char *ppp;
   if ((fp=fopen(fname,"r"))==NULL){
       fprintf(stderr,"Cannot find file \"%s\"\n",fname);
       exit(1);}
   while (fgets(line,82,fp)){
          if (aux_trn_knt >= MAX_TRN_KNT){
              fprintf(stderr,"Too many translations\n"); exit(1);}
          if (ppp=strstr(line,"\n")) *ppp='\0';
          ppp = strstr(line,",");
          if (ppp){
              *ppp = '\0';
              aux_from[aux_trn_knt] = malloc_copy(line);
              ++ppp;
              aux_to[aux_trn_knt] = malloc_copy(ppp);
              ++aux_trn_knt;}}
   fclose(fp);
}

void out_rate(FILE *fp,char *rate,int knt,int equiv_size)
{
   char *ppp;
   char *rrr;
   char *temprate;
   int nnn;
   nnn = strlen(rate);
   if (nnn>500) abort_program();
   temprate = rrr = malloc(64+nnn);
   if (temprate == NULL) abort_program();
   *temprate = '\0';
   ppp = rate;
   if (strncmp(rate,"FAST ",5) == 0){
       ppp += 5;
       rrr = temprate+strlen(temprate);
       sprintf(rrr,"FAST ");}
   if (knt > 0){
       rrr = temprate+strlen(temprate);
       if ((knt % equiv_size)==0){
           if (knt != 1)
               sprintf(rrr,"(%d)*",(int) (knt/equiv_size));}
       else{
           sprintf(rrr,"(%d/%d)*",(int) knt,equiv_size);}}
   rrr = temprate+strlen(temprate);
   sprintf(rrr,"%s;",ppp);
   if (combine_products) combine(temprate);
   fprintf(fp,"%s",temprate);
   free(temprate);
}

int tto_cmp_eqvs(int srceqv1,int desteqv1,int srceqv2,int desteqv2,
                 transition_info_type *tto1,transition_info_type *tto2)
{
   int ord;
   if (srceqv1 < srceqv2) return(-1);
   if (srceqv1 > srceqv2) return(1);
   if (desteqv1 < desteqv2) return(-1);
   if (desteqv1 > desteqv2) return(1);
   ord = strcmp((*tto1).rate,(*tto2).rate);
   if (ord != 0) return(ord);
   ord = strcmp(states[srceqv1].coll_comments,states[srceqv2].coll_comments);
   if (ord != 0) return(ord);
   ord = strcmp(states[desteqv1].coll_comments,states[desteqv2].coll_comments);
   return(ord);
}
