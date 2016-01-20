#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int many_trantos,pipe_input,frc_suffix;

#if defined(UNIX)||defined(LINUX)
extern int fprintf();
extern int system();
#endif

#define acc_F_OK 0

extern unsigned char exists(char *);
unsigned char exists(char *path)
{
   register unsigned char does;
   /*-------------------------------------------------------*/
   does = (unsigned char) ((access(path,acc_F_OK))==0);
   /*-------------------------------------------------------*/
   return(does);
}

extern void doit(char *);
void doit(char *cline)
{
   fprintf(stderr,"> %s\n",cline);
   system(cline);
}

#define QDO() sprintf(cline,tempfmt,also); if (!exists("core")) doit(cline)
#define DO1(fmt) sprintf(tempfmt,fmt,argv[1]); QDO()
#define DO2(fmt) sprintf(tempfmt,fmt,argv[1],argv[1]); QDO()
#define DO3(fmt) sprintf(tempfmt,fmt,argv[1],argv[1],argv[1]); QDO()
#define DO4(fmt) sprintf(tempfmt,fmt,argv[1],argv[1],argv[1],argv[1]); QDO()



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


extern int main(int,char **);
int main(int argc,char **argv)
{
   char cline[1024];
   char tempfmt[1024];
   char also[1024];
   int iarg;
   also[0] = '\0';
   fprintf(stderr,"[indcolcmp 1.1]\n");
   many_trantos = pipe_input = frc_suffix = 0;
   for (iarg=2;iarg<argc;++iarg){
        if (stricmp(&argv[iarg][1],"fs")==0) frc_suffix = 1;
        else if (stricmp(&argv[iarg][1],"pipe")==0) pipe_input = 1;
        else if (stricmp(&argv[iarg][1],"big")==0) many_trantos = 1;
        else if (stricmp(&argv[iarg][1],"many")==0) many_trantos = 1;
        else{
            if (strlen(also)>0) strcat(also," ");
            strcat(also,argv[iarg]);}}
   if (argc>1){
       /*
        *   First do individually modelled.
        */
       if (many_trantos){
           if (pipe_input){
               DO3("many_ind2coll %s.mod -cp -X%s_frc %%s >qqq_%s.mod");}
           else if (frc_suffix){
               DO4("many_ind2coll %s.mod -cp -X%s_frc %%s <%s_frc.mod >qqq_%s.mod");}
           else{
               DO4("many_ind2coll %s.mod -cp -Xind_%s %%s <ind_%s.mod >qqq_%s.mod");}}
       else{
           if (pipe_input){
               DO3("ind2coll %s.mod -cp -X%s_frc %%s >qqq_%s.mod");}
           else if (frc_suffix){
               DO4("ind2coll %s.mod -cp -X%s_frc %%s <%s_frc.mod >qqq_%s.mod");}
           else{
               DO4("ind2coll %s.mod -cp -Xind_%s %%s <ind_%s.mod >qqq_%s.mod");}}
       DO2("coll2sort %%s <qqq_%s.mod >zzz_%s.mod");
       DO2("coll2sort -cp %%s <zzz_%s.mod >ccc_%s.mod");
       DO2("coll2sort %%s <ccc_%s.mod >ind_%s.mod");
       /*
        *   Then do collectively modelled.
        */
       DO4("ind2coll %s.mod -cp %%s -X%s <%s.mod >hhh_%s.mod");
       DO2("coll2sort %%s <hhh_%s.mod >jjj_%s.mod");
#ifdef OBSOLETE
       DO2("coll2sort -cp %%s <%s.mod >jjj_%s.mod");
#endif
       DO2("coll2sort -cp %%s <jjj_%s.mod >sss_%s.mod");
       DO2("coll2sort %%s <sss_%s.mod >col_%s.mod");
       /*
        *   Then take differences
        */
       DO3("diff ind_%s.mod col_%s.mod >%s.moddf");}
   return(0);
}
