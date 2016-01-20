/*
===============================================================================

        Input/output functions required to parse ASSIST and TOTAL command
        line options.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
           3/25/93   Original routines completed (David Boerschlein)

===============================================================================
*/

#include "cm_sys.h"
#include "cm_types.h"
#ifdef UNIX
#include "x_stdio.h"
#endif
#include "clinelib_ext.h"
#include "lio_ext.h"

/*===========================================================================*/
flagtyp process_redirection_string(char *str)
/*
 * This function processes the redirection command line strings >file
 * and <file.
 */
{
   /*------------------------------------------------------------------*/
   if ((*str)=='>'){
      if ((*(str+1))=='>') freopen(str+2,"a",stdout);
      else freopen(str+1,"w",stdout);}
   else if ((*str)=='<')
      freopen(str+1,"r",stdin);
   else return(f_FALSE);
   /*------------------------------------------------------------------*/
   return(f_TRUE);
}
/*===========================================================================*/
void do_shew_parse_command_line_opts(int argc,int arg0,char **argv)
/*
 * This function shows the command line options as if they were being
 * parsed but does not parse them.
 */
{
   int iii;
   /*------------------------------------------------------------------*/
   for (iii=arg0;iii<argc;++iii){
        printf("   argv[%d] = \"%s\"\n",iii,argv[iii]);}
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
static char **temp_argv;
static short temp_argc;
/*===========================================================================*/
void freeup_from_parse_options_file()
{
   short jjj;
   /*------------------------------------------------------------------*/
   if (temp_argv != ((char **) NULL)){
       for (jjj=0;jjj<temp_argc;++jjj) free((void *) temp_argv[jjj]);
       free((void *) temp_argv);}
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
short parse_options_file(char *figfile,char ***temp_argv_out)
{
   char *temp_1_arg;
   char *ppp;
   char *qqq;
   char *qqqovf;
   FILE *fp;
   /*------------------------------------------------------------------*/
   temp_1_arg = (char *) malloc(ARG_MAXLEN_P);
   temp_argv = ((char **) NULL);
   temp_argc=0;
   if (exists(figfile)){
       fp = fopen(figfile,"r");
       if (fp != NULL){
           while (fgets(temp_1_arg,ARG_MAXLEN,fp) != NULL){
                  qqqovf = temp_1_arg+strlen(temp_1_arg);
                  for (qqq=temp_1_arg;qqq<qqqovf;++qqq){
                       for (ppp=qqq;(!isspace(*ppp))&&((*ppp)!='\n')&&((*ppp)!='\0');++ppp);
                       *ppp='\0';
                       if (ppp>qqq){
                           if (temp_argc == 0)
                               temp_argv = (char **) malloc(sizeof(char *));
                           else
                               temp_argv = (char **)
                               realloc(temp_argv,(1+temp_argc)*sizeof(char *));
                           temp_argv[temp_argc] = (char *)
                               malloc(1+strlen(qqq));
                           strcpy(temp_argv[temp_argc],qqq);
                           temp_argc++;}
                       qqq=ppp;}}
           fclose(fp);}}
   free(temp_1_arg);
   /*------------------------------------------------------------------*/
   *temp_argv_out = temp_argv;
   return(temp_argc);
}
/*===========================================================================*/
void set_user_home_directory(char **envp,char *options_config_file)
{
   char **ppp;
   char *qqq;
   /*------------------------------------------------------------------*/
   strcpy(user_home_dir,"");
   for (ppp=envp;(*ppp)!=NULL;++ppp){
#ifdef BUGGY
   printf("%s\n",*ppp);
#endif
        if (strncmp(*ppp,"HOME=",5)==0) strcpy(user_home_dir,(*ppp)+5);}
   strcpy(user_config_file,user_home_dir);
#ifdef UNIX
   qqq=user_config_file+strlen(user_config_file)-1;
   if ((*qqq)!='/') strcat(user_config_file,"/");
#endif
   strcat(user_config_file,options_config_file);
   /*------------------------------------------------------------------*/
}
/*===========================================================================*/
