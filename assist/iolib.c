/*
===============================================================================

        Medium level input/output functions required by ASSIST and SURE 
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

#include "common.h"

#include "astr_ext.h"
#include "objdefs.h"
#include "objtypes.h"
#include "astoptfig.h"

#include "clinelib_ext.h"
#include "io_ext.h"
#include "obj_ext.h"

extern char full_revmsg[];
extern char brief_revmsg[];

#ifdef WINASSIST
#include <setjmp.h>
extern jmp_buf ProcessExit;
#endif

#ifdef BIGSPACE
#define I_RANGE_MAX 32767
#else
#define I_RANGE_MAX 255
#endif

/*
   Diagram of current state of input file:

               ___________________________________
              |           |           |           |
              |  current  | lookahead |  un-read  |
              | character | character | character |
              |___________|___________|___________|
                    |          |             |
                   /_\        /_\           /_\
                    |          |             |
                    |     _____|_____    ____|_________
                    |     line_number    line_buffer_ix
                    |   (line_buffer[])
              ______|_______
              arrow_position
                        
         (*)  "current_ch" and "lookahead_ch" are already read in.
         (*)  "lookahead_ch" is the last character already read in.
         (*)  "line_buffer_ix" points to the next unread character.
         (*)  "line_number" corresponds to the last character already read in.
              It is the number of the line contained in "line_buffer[]".
         (*)  "old_line_number" is the number of the line contained in
              "old_line_buffer[]".
         (*)  "line_buffer" holds the line containing the last character
              already read in ("lookahead_ch").  A new line is not loaded
              until attempting to read the character past the end of the
              current line buffer.
         (*)  "old_line_buffer" contains the non-blank line prior to
              the current "line_buffer".
         (*)  "arrow_position" is set when advancing a character.  It is
              used only when error/warning messages are printed.   The
              invoking routine may change this position but the old
              value should be restored after printing the message.
*/

#include "errtabsP.h"
/**/
typedef struct t__source_line_info
   {
        short last_line_in_error;
        short last_line_in_warning;
        short error_count_this_line;
        short warning_count_this_line;
        short old_line_number;
        short line_number;
        Boolean line_shown_on_screen;
        scanning_character_info_type char_pair;
        char old_line_buffer[LINE_MAXNCH_P];
        char line_buffer[LINE_MAXNCH_P];
        short line_buffer_ix;
        Boolean must_fudge_it;
   } source_line_info_type;
/**/
static source_line_info_type sinfo;
/**/
static Boolean show_severity=FALSE;           /* -SS  (show severity) */
/**/
static Boolean error_message_just_listed;
static Boolean looking_ahead;
static Boolean prompting_for_input_expression;
static short prompting_overhead;
static FILE *holdstream[SAVE_LEVELS];
static source_line_info_type holdsinfo[SAVE_LEVELS];
static FILE *tempinputfile[SAVE_LEVELS];
static short hold_prompting_overhead[SAVE_LEVELS];
static Boolean hold_pm_for_ie[SAVE_LEVELS];
/**/
static Boolean leftside_of_arrow;
static Boolean rightside_of_arrow;
/**/
static char msg_buffer[MSG_BUFFLEN];
static char scratch_msg[92];
/**/
static short echo_inserted_pos,echo_inserted_ptr_pos;
static Boolean echo_inserted_some,echo_inserted_was_word_or_num;
static short echo_inserted_indent=0;
static short wrapping_linknt;
/**/
static short listpage,listline;
static Boolean form_feed_on_first=FORM_FEED_ON_FIRST_PAGE;
/*===========================================================================*/
void init_iolib()
{
   /*-------------------------------------------------------*/
   gen_suppress = FALSE;
   debug_mode = FALSE;
   option_rec.echo=FALSE;
#ifdef ASSIST
   option_rec.onedeath=option_rec.comment=option_rec.comment_stats=TRUE;
   option_rec.trim=FALSE;
#endif
   inputfile=stdin;
   outputfile=stdout;
   bugfile=stderr;
   objectfile=scrfile=listfile=modfile=(FILE *) NULL;
   executing_rules=parsing_input_file=FALSE;
   debug_flagword=DBG_NONE;
   program_currently_aborting=fatal_error=FALSE;
   error_count=warning_count=0;
   highest_severity_encountered=(-1);
   sinfo.last_line_in_error=sinfo.last_line_in_warning=0;
   sinfo.error_count_this_line=sinfo.warning_count_this_line=0;
   default_command_line_options();
   compute_dependent_options();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void init_sinfo()
{
   /*-------------------------------------------------------*/
   init_prsiolib(&sinfo.char_pair);
   /**/
   sinfo.line_buffer_ix=0;
   *sinfo.line_buffer = *sinfo.old_line_buffer = '\0';
   sinfo.old_line_number = sinfo.line_number = 0;
   sinfo.last_line_in_error=sinfo.last_line_in_warning=0;
   sinfo.error_count_this_line=sinfo.warning_count_this_line=0;
   sinfo.line_shown_on_screen=FALSE;
   /**/
   sinfo.must_fudge_it=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void re_init_iolib()
{
   /*-------------------------------------------------------*/
   error_message_just_listed=FALSE;
   looking_ahead=FALSE;
   listline=32000;
   listpage=0;
   skipping_inside_comment=FALSE;
   executing_rules=FALSE;
   parsing_input_file=TRUE;
   prompting_for_input_expression=FALSE;
   prompting_overhead=0;
   init_sinfo();
   end_of_file_reached = FALSE;
   reset_arrow_sides();
   echo_inserted_init();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void reset_arrow_sides()
{
   /*-------------------------------------------------------*/
   leftside_of_arrow = rightside_of_arrow = FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void set_arrow_sides(Boolean left,Boolean right)
{
   /*-------------------------------------------------------*/
   leftside_of_arrow = left;
   rightside_of_arrow = right;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void abort_program()
/*
   This function aborts the program when an error occurs which is so severe
   that it cannot be dealt with in a systematic way or recovered from.
*/
{
   /*-------------------------------------------------------*/
   program_currently_aborting=TRUE;
   show_err_on_both(E_ABORTING,(void *) NULL,EMPTY_TYPE);
#ifdef VBUGGY
show_error_counts_on_both();
iii=0;
iii=1/iii;
iii=sqrt(-1.0);
#endif
   abort_program_quietly();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void abort_program_quietly()
{
   short j;
   /*-------------------------------------------------------*/
   if (modfile != NULL)
      fprintf(modfile,"\nCANNOT RUN DUE TO ERRORS !!!!!\n\n");
   for (j=0;j<SAVE_LEVELS;++j)
      cleanup_temporary_filename(tempinputfilename[j]);
   cleanup_temporary_filename(xref_filename);
   program_currently_aborting=TRUE;
   show_error_counts_on_both();

#ifdef WINASSIST  
   longjmp(ProcessExit,1);
#endif

#ifdef MSDOS
   fclose(stderr);
   fclose(outputfile);
   fclose(modfile);  /* RWB */
   fclose(listfile); /* RWB */
   fclose(errorfile); /* RWB */
   fclose(objectfile); /* RWB */
#else
   exit(1);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
short strip_command_line_value(char *argv,char *argv_suffix)
{
/*
   This function strips off a single command line value with a leading equal
   sign.

   Inputs are:
            argv: full command line option text as delimited by space
                  characters.  Displayed when an error is detected.
     argv_suffix: the substring of argv which begins with the character
                  immmediately following the last character of the 
                  keyword.
*/
   short ans;
   long bigans;
   char *ppp;
   /*-------------------------------------------------------*/
   for (ppp=argv_suffix;((*ppp)!='\0')&&((*ppp)!='=');++ppp);
   if (((*ppp)=='=')&&(isdigit(*(ppp+1)))){
      ++ppp;
      bigans=atol(ppp);
      ans=give_short(bigans,E_CMDLINEVALBIG);}
   else{
      ans=0;
      show_err_on_both(E_BADOPTION,(void *) argv,CHAR_TYPE);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void default_command_line_options()
/*
   This function sets the default command line options before parsing
   arcg,argv[].
*/
{
   /*-------------------------------------------------------*/
   force_statistics=FALSE;        /* -FORCE_STATS */
   pipe_model_to_stdout=FALSE;    /* -PIPE (pipe model) */
   no_generate=FALSE;             /* -NOGEN (no rule generation) */
   no_statistics=FALSE;           /* -NOSTATS (no statistics) */
   memory_map = FALSE;            /* -LOADMAP (memory map) */
   xref_map = FALSE;              /* -XREF (cross reference map) */
   capise_mode = TRUE;            /* -C (case sensitive) */
   max_spacepix_count = 100;      /* -PIC=# (state-space picture count) */
   ReallocBucketsAtTime = REALLOC_BUCKETS_AT_A_TIME; /* -BI=# */
   max_idn_count = MAX_IDN_DFLT;  /* -I=# (ident table limit) */
   max_num_count = MAX_NUM_DFLT;  /* -N=# (number table limit) */
   max_opnd_count = MAX_OP_DFLT;  /* -O=# (max operands/expr) */
   max_expr_count = MAX_XPR_DFLT; /* -E=# (max num. of scratch expressions) */
   max_parm_count = MAX_PRM_DFLT; /* -P=# (function parameter count limit) */
   max_nest_count = MAX_NEST_DFLT;/* -NEST=# (max operands/expr) */
   max_body_count = MAX_BODY_DFLT;/* -B=# (function body token count limit) */
   max_rule_count = MAX_RULE_DFLT;/* -RULE=# (rule count limit) */
   bucket_count=BUCKET_COUNT;     /* -BC=# (bucket_width) */
   bucket_width=BUCKET_WIDTH;     /* -BW=# (bucket_width) */
   multi_echo_on_error = FALSE;   /* -ME (multi_echo) */
   max_output_line_length = 80;
   max_input_line_length = DFLT_MAX_LINE_LENG; /*-WID=# (width of source code line) */
   levels_of_warnings = DEFAULT_WARN_LEV; /* -W=# (# warning levs requested */
   max_errs_each_line=DFLT_MAX_ERRS_PER_LINE; /* -LEL=# (error limit) */
   max_wrns_each_line=DFLT_MAX_WRNS_PER_LINE; /* -LWL=# (warning limit) */
   max_errs=DFLT_MAX_ERR_COUNT;               /* -EL=# (error limit) */
   max_wrns=DFLT_MAX_WRN_COUNT;               /* -WL=# (warning limit) */
   show_severity=FALSE;                       /* -SS  (show severity) */
   three_letter_bracketed_words=FALSE;        /* -WE3 (wrn/err/3-letter) */
   autorun=FALSE;                             /* -AUTORUN (-AUTO) (autorun) */
   batch_mode=FALSE;                          /* -BATCH (-BAT) (batch mode) */
   wrn_err_auto_msg_sizing=FALSE;             /* -WEAUTO (wrn/err/3-letter) */
   simplify_rate_expressions=FALSE;           /* -SRE (simplify rate exprs) */
   tab_delta=DFLT_TAB_DELTA;                  /* -TAB=# */
   list_lines_per_page=DEFAULT_LISTING_LINES_PER_PAGE; /* -LP=# */
   wrap_long_constant_expressions=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
char *command_line_positional_arg(int argc,char **argv,int ixuse)
{
   int iii,ix;
   /*-------------------------------------------------------*/
   for (ix=0,iii=1;iii<argc;++iii){
      if (argv[iii][0]!=COMMANDLINE_OPTION_PREFIX){
         ++ix;
         if (ix==ixuse) return(argv[iii]);}}
   /*-------------------------------------------------------*/
   return((char *) NULL);
}
/*===========================================================================*/
#define CMDLN_OPT(s) (i_strcmp(&single_argv[1],(s))==0)
#define CMDLN_OPTN(s,n) (i_strncmp(&single_argv[1],(s),(n))==0)
/**/
#define CMDLN_VAL(n) strip_command_line_value(single_argv,&single_argv[1+(n)])
/**/
/*===========================================================================*/
extern void interpret_1_command_line_opt(char *);
void interpret_1_command_line_opt(char *single_argv)
{
   /*-------------------------------------------------------*/
   if (CMDLN_OPT("C")) capise_mode=FALSE;
   else if (CMDLN_OPT("PIPE"))
#if defined(UNIX)||defined(LINUX)
      pipe_model_to_stdout=TRUE;
#else
      show_wrn_on_both(E_NOTUNIX,single_argv,CHAR_TYPE,SERIOUS_WARN_LEV);
#endif
   else if (CMDLN_OPT("XTW")) wrap_long_constant_expressions=TRUE;
   else if (CMDLN_OPTN("EXPRESSION_TERM",14))
        wrap_long_constant_expressions=TRUE;
   else if (CMDLN_OPTN("WRAP_LONG",8))
        wrap_long_constant_expressions=TRUE;
   else if (CMDLN_OPT("NOXTW")) wrap_long_constant_expressions=FALSE;
   else if (CMDLN_OPT("NO_XTW")) wrap_long_constant_expressions=FALSE;
   else if (CMDLN_OPTN("NO_EXPRESSION_TERM",17))
        wrap_long_constant_expressions=FALSE;
   else if (CMDLN_OPTN("NO_WRAP_LONG",11))
        wrap_long_constant_expressions=FALSE;
   else if (CMDLN_OPTN("FORCE_STATS",8)) force_statistics=TRUE;
   else if (CMDLN_OPTN("DEBUG",5)) debug_mode=TRUE;
   else if (CMDLN_OPTN("NODEBUG",7)) debug_mode=FALSE;
   else if (CMDLN_OPTN("TOTAL",5)) gen_suppress=TRUE;
   else if (CMDLN_OPTN("NOTOTAL",7)) gen_suppress=FALSE;
   else if (CMDLN_OPT("NOGEN")) no_generate=TRUE;
   else if (CMDLN_OPT("GEN")) no_generate=FALSE;
   else if (CMDLN_OPT("NOSTATS")) no_statistics=TRUE;
   else if (CMDLN_OPT("STATS")) no_statistics=FALSE;
   else if (CMDLN_OPT("MAP")) xref_map=TRUE;
   else if (CMDLN_OPT("XREF")) xref_map=TRUE;
   else if (CMDLN_OPT("LOADMAP")) memory_map=TRUE;
   else if (CMDLN_OPT("NOMAP")) xref_map=FALSE;
   else if (CMDLN_OPT("NOXREF")) xref_map=FALSE;
   else if (CMDLN_OPT("NOLOADMAP")) memory_map=FALSE;
   else if (CMDLN_OPT("ME")) multi_echo_on_error=TRUE;
   else if (CMDLN_OPT("SS")) show_severity=TRUE;
   else if (CMDLN_OPT("SRE")) simplify_rate_expressions=TRUE;
   else if (CMDLN_OPT("WE3")) three_letter_bracketed_words=TRUE;
   else if (CMDLN_OPT("NOME")) multi_echo_on_error=FALSE;
   else if (CMDLN_OPT("NOSS")) show_severity=FALSE;
   else if (CMDLN_OPT("NOSRE")) simplify_rate_expressions=FALSE;
   else if (CMDLN_OPT("NOWE3")) three_letter_bracketed_words=FALSE;
   else goto continue_point;
   return;
continue_point:
   if (CMDLN_OPTN("AUTO",4)) autorun=TRUE;
   else if (CMDLN_OPTN("BAT",3)) batch_mode=TRUE;
   else if (CMDLN_OPTN("WEA",3)) wrn_err_auto_msg_sizing=TRUE;
   else if (CMDLN_OPTN("WID",3)){
        max_input_line_length=CMDLN_VAL(3);
        if (max_input_line_length>LINE_MAXNCH){
           max_input_line_length=LINE_MAXNCH;
           sprintf(scratch_msg,"\"%s\", (limit=%d)",
                   single_argv,(int) LINE_MAXNCH);
           show_err_on_both(E_OPTIONTOOBIG,
                            (void *) scratch_msg,CHAR_TYPE);}
        else if (max_input_line_length<LINE_MINCH){
           max_input_line_length=LINE_MINCH;
           sprintf(scratch_msg,"\"%s\", (limit=%d)",
                   single_argv,(int) LINE_MINCH);
           show_err_on_both(E_OPTIONTOOTINY,
                            (void *) scratch_msg,CHAR_TYPE);}}
   else if (CMDLN_OPTN("TAB",3)){
        tab_delta=CMDLN_VAL(3);
        if (tab_delta<0) tab_delta=NEGFIX_TAB_DELTA;
        if (tab_delta>MAX_TAB_DELTA) tab_delta=MAX_TAB_DELTA;}
   else if (CMDLN_OPTN("NEST",4)) max_nest_count=CMDLN_VAL(4);
   else if (CMDLN_OPTN("RULE",4)) max_rule_count=CMDLN_VAL(4);
   else if (CMDLN_OPTN("PIC",3)) max_spacepix_count=CMDLN_VAL(3);
   else if (CMDLN_OPTN("LEL",3)) max_errs_each_line=CMDLN_VAL(3);
   else if (CMDLN_OPTN("LWL",3)) max_wrns_each_line=CMDLN_VAL(3);
   else if (CMDLN_OPTN("EL",2)) max_errs=CMDLN_VAL(2);
   else if (CMDLN_OPTN("WL",2)) max_wrns=CMDLN_VAL(2);
   else if (CMDLN_OPTN("BI",2)) ReallocBucketsAtTime = CMDLN_VAL(2);
   else if (CMDLN_OPTN("BC",2)) bucket_count=CMDLN_VAL(2);
   else if (CMDLN_OPTN("BW",2)) bucket_width=CMDLN_VAL(2);
   else if (CMDLN_OPTN("LP",2)) list_lines_per_page=CMDLN_VAL(2);
   else if (CMDLN_OPTN("I",1)) max_idn_count=CMDLN_VAL(1);
   else if (CMDLN_OPTN("N",1)) max_num_count=CMDLN_VAL(1);
   else if (CMDLN_OPTN("O",1)) max_opnd_count=CMDLN_VAL(1);
   else if (CMDLN_OPTN("E",1)) max_expr_count=CMDLN_VAL(1);
   else if (CMDLN_OPTN("P",1)) max_parm_count=CMDLN_VAL(1);
   else if (CMDLN_OPTN("B",1)) max_body_count=CMDLN_VAL(1);
   else if (CMDLN_OPT("W=FEWER")) --levels_of_warnings;
   else if (CMDLN_OPT("W=MORE")) ++levels_of_warnings;
   else if (CMDLN_OPT("W=DEFAULT"))
        levels_of_warnings=DEFAULT_WARN_LEV;
   else if (CMDLN_OPT("W=NONE")) levels_of_warnings=NONE_WARN_LEV;
   else if (CMDLN_OPT("W=ALL")) levels_of_warnings=ALL_WARN_LEV;
   else if (CMDLN_OPTN("W",1)) levels_of_warnings=CMDLN_VAL(1);
   else show_err_on_both(E_BADOPTION,(void *) single_argv,CHAR_TYPE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
extern void qq_parse_command_line_options(int,int,char **);
void qq_parse_command_line_options(int argc,int arg0,char **argv)
{
   int iii;
   /*-------------------------------------------------------*/
   for (iii=arg0;iii<argc;++iii){
        if (process_redirection_string(argv[iii]));
        else if (argv[iii][0]==COMMANDLINE_OPTION_PREFIX)
             interpret_1_command_line_opt(argv[iii]);
        else if (!(isascii(argv[iii][0])&&isalpha(argv[iii][0]))){
             show_err_on_both(E_BADOPTION,(void *) argv[iii],CHAR_TYPE);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_command_line_options(int argc,char **argv)
{
   int iii;
   char **temp_argv;
   int temp_argc;
   /*-------------------------------------------------------*/
   temp_argc = parse_options_file(user_config_file,&temp_argv);
   if (temp_argv != ((char **) NULL)){
       qq_parse_command_line_options(temp_argc,0,temp_argv);}
   qq_parse_command_line_options(argc,1,argv);
   if (debug_mode) debug_flagword |= DBG_XPN;
   if (temp_argv != ((char **) NULL)){
       if (debug_mode || bitset(debug_flagword,DBG_MOST)){
           printf("\nParsing \"%s\":\n",user_config_file);
           do_shew_parse_command_line_opts(temp_argc,0,temp_argv);}}
   freeup_from_parse_options_file();
   if (debug_mode){
       printf("\nParsing command line:\n");
       do_shew_parse_command_line_opts(argc,1,argv);
       printf("\n\n");}
   if (batch_mode){
       fprintf(stderr,"\n%s",SYSTEM_PROMPT);
       for (iii=0;iii<argc;++iii){
            fprintf(stderr," ");
            fprintf(stderr,"%s",argv[iii]);}
       fprintf(stderr,"\n");}
   /*-------------------------------------------------------*/
   if (fatal_error) abort_program();
}
/*===========================================================================*/
#ifdef BUGGY
#define for_all_truth(what) for (what=FALSE;what<=TRUE;++what)
void set_debug_options()
{
   levels_of_warnings=99; /* ????? */
#ifdef VBUGGY
   show_severity=TRUE;
   wrn_err_auto_msg_sizing=TRUE;
   max_errs=max_wrns=max_errs_each_line=max_wrns_each_line=32000;
#endif
}
void dump_all_error_messages()
{
   short ee;
   short point_left;
   short point_right;
   for_all_truth(point_left) for_all_truth(point_right){
      for (arrow_position=0;arrow_position<80;arrow_position+=20){
      fprintf(listfile,"\n\n\narrow_position=%d\n",(int) arrow_position);
      fprintf(listfile,"        0         1         2         3 ");
      fprintf(listfile,"        4         5         6         7\n");
      fprintf(listfile,"(0000): *123456789*123456789*123456789");
      fprintf(listfile,"*123456789*123456789*123456789*123456789*\n");
      for_all_truth(wrn_err_auto_msg_sizing) for (ee=0;ee<ERROR_COUNT;++ee){
         show_err_on_both(ee,(void *) NULL,EMPTY_TYPE);
         show_wrn_on_both(ee,(void *) NULL,EMPTY_TYPE,SERIOUS_WARN_LEV);}}}
}
#endif
/*===========================================================================*/
void compute_dependent_options()
/*
   This function initializes variables which are dependent upon the 
   selected language command line options.
*/
{
   long prod;
   /*-------------------------------------------------------*/
   prod=max_parm_count;
   prod *= ((long) max_nest_count);
   if (prod > ((long) MAXSHORT)){
      show_err_on_both(E_NOMEMORYP,(void *) NULL,EMPTY_TYPE);
      abort_program();}
   decide_bugfile();
   mask_out_non_printing=FALSE;
   half_output_line_length = (max_output_line_length/2);
   list_line_width=90;
   max_ops_count=20+2*max_opnd_count;
   max_identifier_table_alloc =  max_identifier_table_count = 
      set_idn_num_counts(max_idn_count,max_num_count);
   current_temporary_count = 0;
   init_error_words(tab_delta,three_letter_bracketed_words,show_severity);
   echo_inserted_init();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void echo_inserted_init()
{
   /*-------------------------------------------------------*/
   echo_inserted_some=FALSE;
   echo_inserted_ptr_pos=0;
   echo_inserted_pos=max_output_line_length+1;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void echo_inserted_force_newline()
{
   /*-------------------------------------------------------*/ 
   if (echo_inserted_some) echo_inserted_newline(0);
   /*-------------------------------------------------------*/ 
}
/*===========================================================================*/
void echo_inserted_newarrow(short incamt)
{
   /*-------------------------------------------------------*/ 
   echo_inserted_token_to_listing("  >>>------->");
   echo_inserted_newline_indent(incamt);
   /*-------------------------------------------------------*/ 
}
/*===========================================================================*/
short set_echo_inserted_indent(short delta_nch)
{
   short old;
   /*-------------------------------------------------------*/
   old = echo_inserted_indent;
   echo_inserted_indent += delta_nch;
   if (echo_inserted_indent<0) echo_inserted_indent=0;
   /*-------------------------------------------------------*/
   return(old);
}
/*===========================================================================*/
short echo_inserted_newline(short delta_nch)
{
   register short n;
   /*-------------------------------------------------------*/
   echo_inserted_some=TRUE;
   set_echo_inserted_indent(delta_nch);
   n=echo_inserted_indent; if (n>40) n=40;
   echo_inserted_ptr_pos=echo_inserted_pos=n;
   fprintf(listfile,"\n");
   /*-------------------------------------------------------*/
   return(n);
}
/*===========================================================================*/
void echo_inserted_newline_final(short delta_nch)
{
   register short n;
   /*-------------------------------------------------------*/
   n=echo_inserted_newline(delta_nch);
   echo_inserted_init();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void echo_inserted_newline_indent(short delta_nch)
{
   register short i,n;
   /*-------------------------------------------------------*/
   n=echo_inserted_newline(delta_nch);
   list_page_need(1);
   fprintf(listfile,"  >>>>: ");
   for (i=0;i<n;++i) fprintf(listfile," ");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void echo_inserted_token_to_listing(char *id)
{
   register short i,n,nch;
   char *what;
   Boolean is_word_or_num;
   /*-------------------------------------------------------*/
   what=id;
   if (is_word_or_num=(Boolean) ((*id)==NUMBER_TABLE_PREFIX_CHAR)) ++what;
   if (isalpha(*id)) is_word_or_num=TRUE;
   nch=strlen(what);
   n=echo_inserted_indent; if (n>40) n=40;
   if ((echo_inserted_pos+nch)>=max_output_line_length){
      if (echo_inserted_some) fprintf(listfile,"\n");
      list_page_need(1);
      fprintf(listfile,"  >>>>: ");
      for (i=0;i<n;++i) fprintf(listfile," ");
      echo_inserted_pos=n;}
   else if (is_word_or_num && echo_inserted_was_word_or_num){
      ++echo_inserted_pos;
      fprintf(listfile," ");}
   echo_inserted_was_word_or_num=is_word_or_num;
   echo_inserted_some=TRUE;
   echo_inserted_ptr_pos = echo_inserted_pos + nch/2;
   echo_inserted_pos += nch;
   fprintf(listfile,"%s",what);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void print_listing_line(short lineno,char *line,FILE *fp)
/*
   This function prints the given line (assumed to terminate with both
   a newline and a null character) in listing file format into the
   file pointed to by "fp".   If "fp" is null then the request is ignored.
*/
{
   short need;
   Boolean to_screen,with_number;
   /*-------------------------------------------------------*/
   if (fp==((FILE *) NULL)) return;
   need=1;
   if (error_message_just_listed){ fprintf(fp,"\n"); ++need; }
   error_message_just_listed=FALSE;
   if (!prompting_for_input_expression){
      to_screen = (Boolean) ((fp==stdout)||(fp==stderr));
      with_number = (Boolean) (!(to_screen && option_rec.echo));
      if (fp==listfile){
         if (echo_inserted_some) echo_inserted_newline_final(0);
         list_page_need(1);}
      if (with_number){
         if ((fp==listfile)&&(skipping_inside_comment))
            fprintf(fp,"(%04d)X",(int) lineno);
         else 
            fprintf(fp,"(%04d):",(int) lineno);
         if (to_screen) fprintf(fp,"\n");
         else fprintf(fp," ");}}
   safe_fprintf(fp,line);
   if ((prompting_for_input_expression)&&(fp==listfile)) list_page_need(need);
   /*-------------------------------------------------------*/
}
#define LISTING_NUMBER_OVERHEAD 8 /* "(%04d): " */
#define STDOUT_NUMBER_OVERHEAD 0  /* "(%04d): " */
/*===========================================================================*/
extern char *where_to_wrap_ovf_pos(char *,char *,short);
char *where_to_wrap_ovf_pos(char *str,char *strovf,short wid_fits)
{
   char register *qq0,*qq,*qqe;
   /*-------------------------------------------------------*/
   qq0=str;
   qq=(qqe=qq0+wid_fits)-1;
   if (qq>=strovf){
      qq=(qqe=strovf)-1;}
   else{
      for (;(qq>qq0)&&(is_text_nowrap_after_char(*qq));--qq);}
   if (qq>qq0) qqe=(qq+1);
   /*-------------------------------------------------------*/
   return(qqe);
}
/*===========================================================================*/
short siglen_wrapping_message
     (FILE *fp,char *msg,short pos,short indent,short wall,short grace)
{
   short register ans,nch;
   /*-------------------------------------------------------*/
   nch=siglen(msg,strlen(msg));
   *(msg+nch) = '\0';
   ans = wrapping_message(fp,msg,pos,indent,wall,grace);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/

short wrapping_message
     (FILE *fp,char *msg,short pos,short indent,short wall,short grace)
/*
   This function displays the message "msg" inserting newline characters in
   order not to exceed the screen width.  Text following inserted newline
   characters is automatically indented "indent+pos" columns.

   The first line of the message is allowed to extend "grace" extra characters
   to the right.

   If "pos" is negative and output is to the terminal, then characters are
   truncated from the beginning of the message to make the message line up.

*/
{
   short register j;
   char register *qq;
   char register *qq9;
   char register *qq0;
   char *msgend;
   short nskip0,nskip,wid,gce,start,overhead;
   char *pp;
   Boolean to_screen,to_model;
   /*-------------------------------------------------------*/
   wrapping_linknt=0;
#if defined(BUGGY)||defined(ZZBUGGY)
fprintf(bugfile,"wrapping_message(fp,msg,pos=%d,indent=%d,wall=%d,grace=%d)\n",
(int) pos,(int) indent,(int) wall,(int) grace);
if ((indent<0)||(wall<(pos+indent+5))){
fprintf(bugfile,
"*** ERR, wrapping_message(fp,msg=\"%s\",pos=%d,indent=%d,wall=%d,grace=%d)\n",
msg,(int) pos,(int) indent,(int) wall,(int) grace);
exit(1);}
#endif
   to_screen=(Boolean) ((fp==stdout)||(fp==stderr));
   to_model=(Boolean) (fp==modfile);
   if (prompting_for_input_expression){
      overhead=nskip0=prompting_overhead;}
   else{
      overhead=nskip0=LISTING_NUMBER_OVERHEAD;
      if ((to_screen)||(to_model)) overhead=nskip0=STDOUT_NUMBER_OVERHEAD;}
   nskip0 += pos;
   nskip=nskip0+indent;
   /*---------------------------------  truncate characters if necessary */
   pp=msg;
   if (nskip0<0) pp -= nskip0;
   /*---------------------------------  skip to beginning of first line */
   for (j=0;j<nskip0;++j) putc(' ',fp);
   /*---------------------------------  print each character of the message */
   start=pos;
   gce=grace;
   msgend=msg+strlen(msg);
   for (qq9=qq0=pp=msg;(*pp)!='\0';){
      wid=wall-start+gce-1;
      qq9=where_to_wrap_ovf_pos(qq0=pp,msgend,wid);

      for (qq=qq0; (qq<qq9) && (*qq!='\0'); ++qq) 
	/* <<<<<<GNU>>>> safe_putc(*qq,fp);   */
       putc(*qq,fp);   
      if ((*qq)!='\0'){
         ++wrapping_linknt;
         putc('\n',fp);
#ifdef VBUGGY
if ((n>3)&&((*p)=='\^'))
fprintf(bugfile,"pos0=%d,indent=%d,wall=%d,grace=%d;n=%d,j=%d,newn=%d\n",
(int) pos0,(int) indent,(int) wall,(int) grace,(int) n,(int) j,(int) newn);
#endif
         for (j=0;j<nskip;++j) putc(' ',fp);}
      for (pp=qq9;(*pp!='\0')&&(isspace(*pp));++pp);
      start=pos+indent;
      gce=0;}
   start = qq9-qq0+indent;
   /*-------------------------------------------------------*/
   return(start);
}
/*===========================================================================*/
void show_err_or_wrn(FILE *fp,
                     short ee,
                     char *err_warn_word,
                     void *aux,        /* NULL if no auxilliary info */
                     type_flagword_type auxtyp)  /* type of aux info */
/*
   This function displays the error or warning message corresponding to
   "ee" on the file given by "fp".   The file can be "stderr" or some other
   text file.   If "fp" is null, then the request is ignored.
*/
{
   short iii,indent,n_abuf,o_abuf,pos,wall,grace,nsp,putpos,save_arrow_pos;
   char *msg_ptr,*ppp,*qqq;
   char abuf[8];
   Boolean arrow_first,must_break,to_screen,extra;
   /*-------------------------------------------------------*/
   must_break=(Boolean) ((fp==listfile)&&(echo_inserted_some));
   save_arrow_pos=arrow_position;
   if (must_break) arrow_position=echo_inserted_ptr_pos;
   if (arrow_position<0) arrow_position=0;
#if defined(BUGGY) || defined(ZZBUGGY)
if ((arrow_position>100)||(arrow_position<0)){
fprintf(stderr,"bad arrow position = %d\n",(int) arrow_position);
arrow_position=0;
arrow_position=1/arrow_position;
abort_program();}
#endif
   *msg_buffer = '\0';
   if (fp==((FILE *) NULL)) goto done_point;
   if (ee==0) goto done_point;
   arrow_first = (Boolean) (arrow_position < half_output_line_length);
   pos=indent=grace=0;
   wall=max_output_line_length;
   err_or_warn_last_indented0 = indent = 3 + strlen(err_warn_word);
   if (!parsing_input_file) goto encode_message_point;
   /*------------------------------------------  echo to stdout if not yet */
   to_screen=(Boolean) ((fp==stdout)||(fp==stderr));
   if (((!to_screen)&&(fp!=listfile))||
       ((to_screen)&&(!sinfo.line_shown_on_screen))){
      if (fp!=listfile) fprintf(fp,"\n");
      if (to_screen) sinfo.line_shown_on_screen=TRUE;
      if ((fp!=listfile)&&(test_if_char_is_first_on_line()))
         print_listing_line(sinfo.old_line_number,sinfo.old_line_buffer,fp);
      print_listing_line(sinfo.line_number,sinfo.line_buffer,fp);}
   /*------------------------------------------  build pointer to error */
   encode_arrow_to_error(abuf,&n_abuf,&o_abuf,
                         leftside_of_arrow,
                         rightside_of_arrow);
   /*------------------------------------------  compute message window */
   grace = 0;
   if (arrow_first){
      pos = arrow_position;
      wall = pos + half_output_line_length;
      if (wrn_err_auto_msg_sizing) wall = max_output_line_length;
      pos -= o_abuf;}
   else{
      grace = n_abuf-o_abuf;
      wall = arrow_position;
      pos = wall - half_output_line_length;
      if (wrn_err_auto_msg_sizing) pos = 0;}
#ifdef TEST_BUGGY
if ((wall<=pos)&&(!program_currently_aborting)){
program_currently_aborting=TRUE;
show_err_on_both(E_INTERNAL,"wall<=pos (show_err_or_wrn)",CHAR_TYPE);
abort_program();}
if ((wall>max_output_line_length)&&(!program_currently_aborting)){
program_currently_aborting=TRUE;
show_err_on_both(E_INTERNAL,"wall too big (show_err_or_wrn)",CHAR_TYPE);
abort_program();}
if ((pos<0)&&(!program_currently_aborting)){
program_currently_aborting=TRUE;
show_err_on_both(E_INTERNAL,"pos is negative (show_err_or_wrn)",CHAR_TYPE);
abort_program();}
if ((wall<=pos)||(wall>max_output_line_length)||(pos<0)){
arrow_position=pos=0;
wall=max_output_line_length;}
#endif
   /*------------------------------------------  build error message */
   /*--------------------------  maybe "^" first */
   if (arrow_first){
      indent += n_abuf;
      strcat(msg_buffer,abuf);}
   /*--------------------------  [ERROR] or [WARNING] */
encode_message_point:
   strcat(msg_buffer,"[");
   strcat(msg_buffer,err_warn_word);
   strcat(msg_buffer,"] ");
   /*--------------------------  text of error message */
   if ((ee>=0)&&(ee<ERROR_COUNT))
      safe_strcat_truncated(msg_buffer,errmsg_array[ee],MSG_BUFFLEN);
   else{
      iii=strlen(msg_buffer);
      msg_ptr = msg_buffer + iii;
      sprintf(msg_ptr,"Condition number %d raised.",(int) ee);}
   /*------------------------------------------  encode & append extra info */
   extra = (Boolean) (aux != (NULL_PTR));
   if (extra && (auxtyp==CHAR_TYPE) && ((*((char *) aux))=='\0')) extra=FALSE;
   if (extra){
      msg_ptr = enk_aux_val(aux,auxtyp);
      iii=strlen(msg_buffer);
      if (!((*(msg_buffer+iii-1)=='.')&&(*(msg_buffer+iii-2)=='.'))){
         /*--------------------------  no subrange, insert ':' and/or ' ' */
         if (*(msg_buffer+iii-1)=='.')
            *(msg_buffer+iii-1)=':';
         else{
            *(msg_buffer+(iii++))=':';
            *(msg_buffer+iii)='\0';}
         safe_strcat_truncated(msg_buffer," ",MSG_BUFFLEN);}
      safe_strcat_truncated(msg_buffer,msg_ptr,MSG_BUFFLEN);}
   /*--------------------------  maybe "^" last */
   if ((!arrow_first) && (parsing_input_file)){
      --n_abuf;
      /*-------------  position carat before newline */
      putpos=(wall-pos)-o_abuf;
      if (putpos>(iii=strlen(msg_buffer))){
         safe_strcat_truncated(msg_buffer," ",MSG_BUFFLEN);
         putpos=iii+1;
         pos=wall-(putpos+o_abuf);}
      /*-------------  don't split a word */
      nsp=0;
      qqq=ppp=msg_buffer;
      for (ppp+=(putpos-1);(!isspace(*ppp));--ppp,++nsp);
      ++ppp;
      /*-------------  make room for carat */
      qqq += strlen(msg_buffer);
      for (;qqq>=ppp;--qqq) *(qqq+nsp+n_abuf) = *qqq;
      /*-------------  insert carat (right justified) */
      for (iii=0;iii<nsp;++iii) *ppp++ = ' ';
      sys_memcpy(ppp,abuf,n_abuf);}
   /*------------------------------------------  print error message */
   if (fp==listfile) error_message_just_listed=TRUE;
   if (must_break) fprintf(fp,"\n");
   wrapping_message(fp,msg_buffer,pos,indent,wall,grace);
   if (must_break) echo_inserted_newline_indent(0);
   else fprintf(fp,"\n");
   if (fp==listfile) list_page_need(wrapping_linknt);
   /*-------------------------------------------------------*/
done_point:
   arrow_position=save_arrow_pos;
}
/*===========================================================================*/
#define if_inc_1_yields_too_many(x,xmax) \
        ++x; if ((x>xmax)&&(!program_currently_aborting))
/*===========================================================================*/
void error_bump()
{
   int_type val;
   /*-------------------------------------------------------*/
   if (sinfo.last_line_in_error!=sinfo.line_number){
      sinfo.last_line_in_error=sinfo.line_number;
      sinfo.error_count_this_line=0;}
   if_inc_1_yields_too_many(error_count,max_errs){
      if (parsing_input_file || executing_rules){
         val=max_errs;
         program_currently_aborting=TRUE;
         show_err_on_both(E_TOOMANYERRS,(void *) &val,INT_TYPE);
         abort_program();}}
   if_inc_1_yields_too_many(sinfo.error_count_this_line,max_errs_each_line){
      if (parsing_input_file){
         val=max_errs_each_line;
         program_currently_aborting=TRUE;
         show_err_on_both(E_TOOMANYERRSTHIS,(void *) &val,INT_TYPE);
         abort_program();}}
   fatal_error=TRUE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void warning_bump(short lev)
{
   int_type val;
   /*-------------------------------------------------------*/
   if (sinfo.last_line_in_warning!=sinfo.line_number){
      sinfo.last_line_in_warning=sinfo.line_number;
      sinfo.warning_count_this_line=0;}
   if (lev>highest_severity_encountered) highest_severity_encountered=lev;
   if_inc_1_yields_too_many(warning_count,max_wrns){
      if (parsing_input_file || executing_rules){
         val=max_wrns;
         program_currently_aborting=TRUE;
         show_err_on_both(E_TOOMANYWRNS,(void *) &val,INT_TYPE);
         abort_program();}}
   if_inc_1_yields_too_many(sinfo.warning_count_this_line,max_wrns_each_line){
      if (parsing_input_file){
         val=max_wrns_each_line;
         program_currently_aborting=TRUE;
         show_err_on_both(E_TOOMANYWRNSTHIS,(void *) &val,INT_TYPE);
         abort_program();}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
int_type show_int_err(short ee,int_type iii)
{
   int_type value;
   /*-------------------------------------------------------*/
   value=iii;
   show_err_on_both(ee,&value,INT_TYPE);
   /*-------------------------------------------------------*/
   return(0);
}
/*===========================================================================*/
real_type show_real_err(short ee,real_type iii)
{
   real_type value;
   /*-------------------------------------------------------*/
   value=iii;
   show_err_on_both(ee,&value,REAL_TYPE);
   /*-------------------------------------------------------*/
   return(ZERO);
}
/*===========================================================================*/
void show_err(FILE *fp,short ee,void *aux,type_flagword_type auxtyp)
/*
   This function displays the error message corresponding to "ee" on the file
   given by "fp".   The file can be "stdin" or some other text file.
*/
{
   /*-------------------------------------------------------*/
   if (ee==0) return;
   show_err_or_wrn(fp,ee,unbracketed_err_word,aux,auxtyp);
   error_bump();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_wrn
     (FILE *fp,short ee,void *aux,type_flagword_type auxtyp,short level)
/*
   This function displays the warning message corresponding to "ee" on the file
   given by "fp".   The file can be "stdin" or some other text file.
*/
{
   char wrnmsg[22];
   /*-------------------------------------------------------*/
   if (ee==0) return;
   if (level<=levels_of_warnings){
      encode_warning_and_severity(level,wrnmsg);
      show_err_or_wrn(fp,ee,wrnmsg,aux,auxtyp);
      warning_bump(level);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void wrapping_message_on_both(char *msg)
{
   short pos;
   Boolean erropen,lisopen;
   /*-------------------------------------------------------*/
   erropen = (Boolean) (errorfile != ((FILE *) NULL));
   lisopen = (Boolean) (listfile != ((FILE *) NULL));
   pos = err_or_warn_last_indented0;
   if (erropen)
       wrapping_message(errorfile,msg,pos,pos,max_output_line_length,0);
   if (lisopen){
      wrapping_message(listfile,msg,pos,pos,max_output_line_length,0);
      list_page_need(wrapping_linknt);}
   if (!(erropen||lisopen))
      wrapping_message(stderr,msg,pos,pos,max_output_line_length,0);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_with_fmt_on_both(char *fmt,short val,char *s)
{
   Boolean erropen,lisopen;
   char *plural;
   static char schar[] = "S";
   static char nullchar[] = "";
   /*-------------------------------------------------------*/
   plural = ((val==1) ? nullchar : schar);
   erropen = (Boolean) (errorfile != ((FILE *) NULL));
   lisopen = (Boolean) (listfile != ((FILE *) NULL));
   if (erropen) fprintf(errorfile,fmt,(int) val,s,plural);
   if (lisopen) fprintf(listfile,fmt,(int) val,s,plural);
   if (!(erropen||lisopen)) fprintf(stderr,fmt,(int) val,s,plural);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
extern void show_prs_sizes_on_both(void);
#define SAKMOD(kkk,sss) \
   if (modfile != ((FILE *) NULL)) \
      fprintf(modfile,fmt_comment_counts,(int) kkk,sss,((kkk==1) ? "" : "S"))
void show_error_counts_on_both(void)
{
   static char fmt_counts[] = "%04d %s%s.\n";
   static char fmt_comment_counts[] = "(* %04d %s%s *)\n";
   short need;
   /*-------------------------------------------------------*/
   if (listfile!=((FILE *) NULL)){
      need=2;
      if (warning_count>0) need += 2;
      list_page_need(need);}
   both_printf("\n");
   if (error_count>0){
      SAKMOD(error_count,"ERROR");
      show_with_fmt_on_both(fmt_counts,error_count,"ERROR");}
   if (warning_count>0){
      SAKMOD(warning_count,"WARNING");
      show_with_fmt_on_both(fmt_counts,warning_count,"WARNING");
      if (bitset(debug_flagword,DBG_MOST))
         show_with_fmt_on_both(
                "MOST INSIGNIFICANT WARNING DETECTED IS OF LEVEL = %d%s\n",
                highest_severity_encountered,".");}
   show_prs_sizes_on_both();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_err_on_both(short ee,void *aux,type_flagword_type auxtyp)
/*
   This function displays the error message corresponding to "ee" on both the
   output and listing files.
*/
{
   Boolean erropen,lisopen;
   /*-------------------------------------------------------*/
   if (ee==0) return;
   erropen = (Boolean) (errorfile != ((FILE *) NULL));
   lisopen = (Boolean) (listfile != ((FILE *) NULL));
   if (erropen) show_err_or_wrn(errorfile,ee,unbracketed_err_word,aux,auxtyp);
   if (lisopen) show_err_or_wrn(listfile,ee,unbracketed_err_word,aux,auxtyp);
   if (!(erropen||lisopen))
      show_err_or_wrn(stderr,ee,unbracketed_err_word,aux,auxtyp);
   error_bump();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_wrn_on_both
     (short ee,void *aux,type_flagword_type auxtyp,short level)
/*
   This function displays the warning message corresponding to "ee" on both the
   output and listing files.
*/
{
   char wrnmsg[22];
   Boolean erropen,lisopen;
   /*-------------------------------------------------------*/
   if (ee==0) return;
   if (level<=levels_of_warnings){
      encode_warning_and_severity(level,wrnmsg);
      erropen = (Boolean) (errorfile != ((FILE *) NULL));
      lisopen = (Boolean) (listfile != ((FILE *) NULL));
      if (erropen) show_err_or_wrn(errorfile,ee,wrnmsg,aux,auxtyp);
      if (lisopen) show_err_or_wrn(listfile,ee,wrnmsg,aux,auxtyp);
      if (!(erropen||lisopen)) show_err_or_wrn(stderr,ee,wrnmsg,aux,auxtyp);
      warning_bump(level);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void save_old_line()
/*
   If the current line is non-blank, then this function copies it to the
   old line.  It can be invoked before reading a new line and/or upon
   end of file.
*/
{
   /*-------------------------------------------------------*/
   if (!is_a_blank_line(sinfo.line_buffer)){
      sinfo.old_line_number=sinfo.line_number;
      strcpy(sinfo.old_line_buffer,sinfo.line_buffer);}
   sinfo.line_buffer_ix=nullify_string(sinfo.line_buffer);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean buffered_load_line(FILE *fp)
/*
   This function loads a new line from "fp" into the line buffer.  Trailing
   blanks before the newline character are removed.  Tab characters are also
   translated to the appropriate number of spaces.  The newline character is
   retained as the last character in the line.
   Long lines are truncated.
*/
{
   short nch,ich;
   Boolean huge_trunced,tab_trunced,trunced,toolong;
   /*-------------------------------------------------------*/
   /*-----------------------------------  first save old line */
   save_old_line();
   /*-----------------------------------  read new line */
   ++sinfo.line_number;
   sinfo.line_buffer[0]='\0';
   if (fgets(sinfo.line_buffer,LINE_MAXNCH_P,fp)==((char *) NULL)){
      return(TRUE);}
   nch=strlen(sinfo.line_buffer);
   if ((nch<LINE_MAXNCH_P)&&(nch>0)&&(sinfo.line_buffer[nch-1]!='\n')){
      sinfo.line_buffer[nch]='\n';
      ++nch;
      sinfo.line_buffer[nch]='\0';}
   /*-----------------------------------  skip to newline char */
   if (bitset(debug_flagword,DBG_MOST)){
      printf("load line: nch=%d ",(int) nch);
      for (ich=0;ich<nch;++ich){
         if (sinfo.line_buffer[ich] < 32)
              printf("^%c",(char) (64+sinfo.line_buffer[ich]));
         else printf("%c",sinfo.line_buffer[ich]);}
      printf("\n");}
   huge_trunced=(Boolean) ((nch<1)||(sinfo.line_buffer[nch-1]!='\n'));
   if (huge_trunced){
      while (((ich=fgetc(fp))!=EOF)&&(((char) ich)!='\n'));}
   /*-----------------------------------  translate tab characters */
   nch=strip_off_trailing_space(sinfo.line_buffer);
   nch=translate_tab_characters(sinfo.line_buffer,nch);
   tab_trunced=(Boolean) (nch>LINE_MAXNCH);
   if (tab_trunced) *(sinfo.line_buffer+(--nch))='\0';
   nch=strip_off_trailing_space(sinfo.line_buffer);
   /*-----------------------------------  add newline */
   add_newline(sinfo.line_buffer,nch);
   /*-----------------------------------  see if line needs to be truncated */
   toolong = (Boolean) (nch>max_input_line_length);
   if (toolong) nch=max_input_line_length;
   trunced = (Boolean) (huge_trunced || tab_trunced || toolong);
   /*-----------------------------------  print line to listing */
   if (!looking_ahead){
      print_listing_line(sinfo.line_number,sinfo.line_buffer,listfile);
      if (option_rec.echo)
         print_listing_line(sinfo.line_number,sinfo.line_buffer,errorfile);}
#ifdef BUGGY
if (bugfile!=listfile)
print_listing_line(sinfo.line_number,sinfo.line_buffer,bugfile);
#endif
   sinfo.line_shown_on_screen = (Boolean) (option_rec.echo != 0);
   /*-----------------------------------   */
   sinfo.line_buffer_ix=0;
   /*-----------------------------------  error if line truncated */
   if (trunced){
      arrow_position=0;
      show_wrn_on_both(E_LINEBIG,NULL_PTR,EMPTY_TYPE,SERIOUS_WARN_LEV);}
   /*-------------------------------------------------------*/
   return(FALSE);
}
/*===========================================================================*/
int buffered_getc(FILE *fp)
/*
   This function pulls the next character from the line buffer.  If the
   line buffer is empty, a new line is loaded from "inputfile".
*/
{
   Boolean eof;
   int register ans;
   /*-------------------------------------------------------*/
   eof = (Boolean) (fp==((FILE *) NULL));
   if (eof) save_old_line();
   while ((!eof) && (sinfo.line_buffer[sinfo.line_buffer_ix] == '\0'))
      eof=buffered_load_line(fp);
   if (eof && (sinfo.line_buffer[sinfo.line_buffer_ix] == '\0')) return(EOF);
   ans=(int) sinfo.line_buffer[sinfo.line_buffer_ix++];
   /*-------------------------------------------------------*/
   return(ans);
 }
/*===========================================================================*/
char readch(char prevch)
/*
   This function reads the next character from the open stream "inputfile".
   The stream can either be "stdin" or some other file.   If stdin is used
   and the previous character is a new-line, then the prompt is displayed.
*/
{
   int ichar;
   char ansch;
   Boolean interactive;
   /*-------------------------------------------------------*/
   if (end_of_file_reached) return(EOF_CHAR);
   interactive=(Boolean) (inputfile==stdin);
#if defined(SURE)||defined(STEM)||defined(PAWS)
   if (interactive && (prevch==NEWLINE)) fprintf(stderr,"%s ",PROMPT_STR);
#endif
   ichar=buffered_getc(inputfile);
   if (ichar==EOF){
      end_of_file_reached=TRUE;
      clearerr(inputfile);
      ansch=EOF_CHAR;}
   else{
      ansch=(char) ichar;}
   /*-------------------------------------------------------*/
   return (ansch);
}
/*===========================================================================*/
void switch_to_stdin(char *prompt_str,short ixlevel)
{
   int ichar;
   Boolean first_time,was_esc,any;
   static int esc_char = (int) '\\';
   static int newline = (int) '\n';
   /*-------------------------------------------------------*/
   make_temporary_filename(tempinputfilename[ixlevel]);
   hold_prompting_overhead[ixlevel]=prompting_overhead;
   hold_pm_for_ie[ixlevel]=prompting_for_input_expression;
   /*---------------------------------------  print prompt message to both */
   list_page_need(2); /* accounts for extra line when switching back */
   both_printf(prompt_str);
   prompting_overhead=strlen(prompt_str);
   /*---------------------------------------  copy from stdin to temp file */
   tempinputfile[ixlevel]=open_output_file(tempinputfilename[ixlevel]);
   first_time=TRUE;
   any=was_esc=FALSE;
   for (ichar=getchar();(ichar!=EOF)&&(ichar!=SEMICOLON);ichar=getchar()){
      if ((!was_esc) && (any) && (ichar==newline)) goto loop_end;
      was_esc=FALSE;
      if (first_time && ((isascii(ichar))&&(isspace(ichar))));
      else if (ichar==esc_char) was_esc=TRUE;
      else{
         if (!is_iolib_space((char) ichar)) any=TRUE;
         putc(ichar,tempinputfile[ixlevel]);
         first_time=FALSE;}}
loop_end:
   putc((int) SEMICOLON,tempinputfile[ixlevel]);
   putc((int) '\n',tempinputfile[ixlevel]);
   fclose(tempinputfile[ixlevel]);
   /*---------------------------------------  switch to temp file */
   tempinputfile[ixlevel]=open_input_file(tempinputfilename[ixlevel]);
   sys_memcpy((char *) &holdsinfo[ixlevel],
              (char *) &sinfo,sizeof(source_line_info_type));
   init_sinfo();
   holdstream[ixlevel]=inputfile;
   inputfile=tempinputfile[ixlevel];
   prompting_for_input_expression=TRUE;
   /*-------------------------------------------------------*/
}      
/*===========================================================================*/
void switch_back_to_inputfile(short ixlevel)
{
   /*-------------------------------------------------------*/
   prompting_overhead=hold_prompting_overhead[ixlevel];
   prompting_for_input_expression=hold_pm_for_ie[ixlevel];
   inputfile=holdstream[ixlevel];
   sys_memcpy((char *) &sinfo,
              (char *) &holdsinfo[ixlevel],sizeof(source_line_info_type));
   restore_prsiolib(&sinfo.char_pair);
   fclose(tempinputfile[ixlevel]);
   cleanup_temporary_filename(tempinputfilename[ixlevel]);
   tempinputfile[ixlevel]=((FILE *) NULL);
   strcpy(tempinputfilename[ixlevel],"");
   /* no call to list_page_need because accounted for in previous routine */
   both_printf("\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
char readch_and_cleanup(char prevch)
/*
   This function reads the next character from the input stream, performing
   the following cleanup if necessary:

        (*) Program aborts on end-of-file on standard input.
        (*) elapsed time is displayed on end-of-file on disk file.
        (*) stream is switched to standard input on end-of-file on disk file.
        (*) end-of-file is replaced with a space character.
        (*) '\t' and '\n' characters are untranslated.
        (*) non-printing characters are replaced with spaces if
            "mask_out_non_printing" is set.

*/
{
   char inch,ansch;
   Boolean interactive;
   /*-------------------------------------------------------*/
   if (end_of_file_reached) return(' ');
   interactive=(Boolean) (inputfile==stdin);
   inch=ansch=readch(prevch);
   if (ansch==EOF_CHAR){
      if (interactive){
         fprintf(stderr,"\n*** USER ABORT ***\n");
         exit(-1);}
      else{
#ifdef FUTURE
????????????????
????????????????   Probably want to do this after parsing has
????????????????   completed.
????????????????
????????????????
#endif
         fclose(inputfile);
         inputfile=(interactive_language ? stdin : ((FILE *) NULL));}
      inch=ansch=' ';}
   else if (ansch=='\t') ;
   else if (ansch=='\n') ;
   else if (mask_out_non_printing&&(!isprint(ansch))) inch=ansch=' ';
   /*-------------------------------------------------------*/
   return(ansch);
}
/*===========================================================================*/
void user_advance_1_char(scanning_character_info_type *charinfo)
/*
   This function copies the lookahead character to the current character
   and reads in a new lookahead character.   Upon end-of-file, the current
   character will be the last character in the file and the lookup character
   will be the space corresponding to the end-of-file.

   Note:  This routine fudges the end-of-line so that both the current
          and lookahead characters are newlines when an end of line occurs.
          This prevents advancing to a new line before returning the last token
          and printing any error messages related to it.  After a fudge
          occurs, the next invocation of this routine reads two characters
          at once to make up for the fact that no characters were read during
          the fudge.
*/
{
   Boolean previous_was_fudged;
   /*-------------------------------------------------------*/
   previous_was_fudged = sinfo.must_fudge_it;
   sinfo.must_fudge_it = (Boolean)
       ((isascii(sinfo.char_pair.current_ch))&&
        (!isspace(sinfo.char_pair.current_ch))&&
        (sinfo.char_pair.lookahead_ch=='\n'));
   /**/
   sinfo.char_pair.current_ch=sinfo.char_pair.lookahead_ch;
   sinfo.char_pair.current_ch_lno=sinfo.char_pair.lookahead_ch_lno;
   sinfo.char_pair.current_ch_pos=sinfo.char_pair.lookahead_ch_pos;
   /**/
   if (previous_was_fudged){
      /*--------------------------------- read both chars from next line */
      prompting_overhead=0;
      sinfo.char_pair.current_ch=readch_and_cleanup(sinfo.char_pair.current_ch);
      sinfo.char_pair.current_ch_lno=sinfo.line_number;
      sinfo.char_pair.current_ch_pos=sinfo.line_buffer_ix-1;
      sinfo.char_pair.lookahead_ch=readch_and_cleanup(sinfo.char_pair.current_ch);
      sinfo.char_pair.lookahead_ch_lno=sinfo.line_number;
      sinfo.char_pair.lookahead_ch_pos=sinfo.line_buffer_ix-1;}
   else if (sinfo.must_fudge_it)
      /*--------------------------------- postpone loading next line */
      sinfo.char_pair.lookahead_ch='\n';
   else{
      /*--------------------------------- middle of line or many empty lines*/
      sinfo.char_pair.lookahead_ch=readch_and_cleanup(sinfo.char_pair.lookahead_ch);
      sinfo.char_pair.lookahead_ch_lno=sinfo.line_number;
      sinfo.char_pair.lookahead_ch_pos=sinfo.line_buffer_ix-1;}
   /**/
   arrow_position=sinfo.line_buffer_ix-2;
   if (arrow_position<0) arrow_position=0;
   sys_memcpy((char *) charinfo,(char *) &sinfo.char_pair,
              sizeof(scanning_character_info_type));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void both_printf(char *s)
{
   Boolean erropen,lisopen;
   /*-------------------------------------------------------*/
   erropen = (Boolean) (errorfile != ((FILE *) NULL));
   lisopen = (Boolean) (listfile != ((FILE *) NULL));
   if (erropen) safe_fprintf(errorfile,s);
   if (lisopen) safe_fprintf(listfile,s);
   if (!(erropen||lisopen)) safe_fprintf(stderr,s);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void prompt_for_prefix_if_missing(char *prompt,char *suffix,char *argv)
{
   /*-------------------------------------------------------*/
   if (argv==NULL){
      fprintf(stderr,"%s: ",prompt);
      fgets(filename_prefix,(2+MAXNCH_PATH),stdin);}
   else if (strlen(argv)>MAXNCH_PATH)
      show_err(stderr,E_FILENAMEBIG,(void *) argv,CHAR_TYPE);
#ifdef FUTURE_MAYBE
      goto too_long;
#endif
   else{
      strcpy(filename_prefix,argv);
      if (i_strcmp(argv,"ast")==0) fprintf(stderr,"%s\n",filename_prefix);}
   if (remove_extent(suffix,filename_prefix)) abort_program();
   /*-------------------------------------------------------*/
   return;
}
/*===========================================================================*/
static Boolean scan_first_time;
static int scan_ichar;
static char scan_ch;
static char scan_och;
/*===========================================================================*/
void tok_after_matching_right_paren(char *id,Boolean matching_already_scanned)
{
   File_offset_t nowpos;
   char *ppp,*pppovf;
   source_line_info_type now_sinfo;
   Boolean again;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,
"\nscan for tok after matching right (mathing_already_scanned=%s)::>>> ",
((matching_already_scanned) ? "TRUE" : "FALSE"));
#endif
   nowpos=ftell(inputfile);
   sys_memcpy((char *) &now_sinfo,(char *) &sinfo,sizeof(source_line_info_type));
   looking_ahead=TRUE;
   scan_och=' ';
   scan_ichar=scan_ch=sinfo.char_pair.current_ch;
   scan_first_time=TRUE;
#ifdef BUGGY
fprintf(bugfile,"%c[",scan_ch);
#endif
   if (!matching_already_scanned){
      while ((scan_ch!=CLOSEPAREN)&&(scan_ch!=SEMICOLON)){
         scan_skip_if_special();
         scan_next_char();}}
#ifdef BUGGY
fprintf(bugfile,"]");
#endif
   if (scan_ch==SEMICOLON) strcpy(id,";");
   else{
      if (!matching_already_scanned) scan_next_char();
      for (again=TRUE;again;){
           while (is_iolib_space(scan_ch)) scan_next_char();
           if (again=scan_skip_if_comment()) scan_next_char();}
#ifdef BUGGY
fprintf(bugfile,"\n");
#endif
      pppovf=id+IDENT_MAXNCH;
      for (ppp=id;(ppp<pppovf)&&(!is_iolib_space(scan_ch));++ppp){
         *ppp=scan_ch;
         scan_next_char();}
      *ppp='\0';
      if (!((isascii(*id))&&(isalnum(*id)))) lookup_as_symbolic_token(id);}
#ifdef BUGGY
fprintf(bugfile,", id=\"%s\"\n",id);
#endif
   looking_ahead=FALSE;
   sys_memcpy((char *) &sinfo,(char *) &now_sinfo,sizeof(source_line_info_type));
   fseek(inputfile,nowpos,SEEK_SET);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void scan_next_char()
{
   /*-------------------------------------------------------*/
   scan_och=scan_ch;
   if (scan_first_time){
      scan_ch=sinfo.char_pair.lookahead_ch;
      scan_ichar=scan_ch;}
   else{
      scan_ichar=buffered_getc(inputfile);
      scan_ch=equiv_text_char(scan_ichar);
      if (scan_ichar==EOF) scan_ch=SEMICOLON;}
   scan_first_time=FALSE;
#ifdef BUGGY
fprintf(bugfile,"%c",scan_ch);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean scan_skip_if_comment()
{
   File_offset_t nowpos;
   source_line_info_type now_sinfo;
   /*-------------------------------------------------------*/
   if (scan_ch==OPENPAREN){
       nowpos=ftell(inputfile);
       sys_memcpy((char *) &now_sinfo,
                  (char *) &sinfo,
                  sizeof(source_line_info_type));
       scan_next_char();
       if (scan_ch==ASTERISK) scan_over_comment(OPENPAREN,CLOSEPAREN);
       else{
           sys_memcpy((char *) &sinfo,
                      (char *) &now_sinfo,
                      sizeof(source_line_info_type));
           fseek(inputfile,nowpos,SEEK_SET);
           return(FALSE);}}
   else if (scan_ch==OPENCURLY) scan_over_comment(OPENCURLY,CLOSECURLY);
   else return(FALSE);
   /*-------------------------------------------------------*/
   return(TRUE);
}
/*===========================================================================*/
Boolean scan_skip_if_special()
{
   /*-------------------------------------------------------*/
   if (scan_ch==OPENPAREN){
       scan_next_char();
       if (scan_ch==ASTERISK) scan_over_comment(OPENPAREN,CLOSEPAREN);
       else finish_scan_to_matching(CLOSEPAREN);}
   else if (scan_ch==OPENCURLY) scan_over_comment(OPENCURLY,CLOSECURLY);
   else if (scan_ch==OPENBRACKET) scan_to_matching(CLOSEBRACKET);
   else return(FALSE);
   /*-------------------------------------------------------*/
   return(TRUE);
}
/*===========================================================================*/
void scan_to_matching(char match)
/*
   This function returns the character which matches "match" at the 
   appropriate nesting level unless a semicolon is encountered and returned.
*/
{
   /*-------------------------------------------------------*/
   scan_next_char();
   finish_scan_to_matching(match);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void finish_scan_to_matching(char match)
/*
   This function returns the character which matches "match" at the 
   appropriate nesting level unless a semicolon is encountered and returned.
*/
{
   /*-------------------------------------------------------*/
   while ((scan_ch!=match)&&(scan_ch!=CLOSEPAREN)&&(scan_ch!=SEMICOLON)){
      scan_skip_if_special();
      scan_next_char();}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void scan_over_comment(char bgnch,char endch)
/*
   This function returns the last character of the ending comment delimiter
   or the EOF character.
*/
{
   char trmch,midch;
   Boolean single_char;
   /*-------------------------------------------------------*/
   midch=ASTERISK; trmch=endch;
   single_char=(Boolean) (endch==CLOSECURLY);
   /*--------------------------------- skip over comment text */
   scan_next_char();
   while (scan_ichar!=EOF){
      if ((single_char)&&(scan_ch==trmch)) return;
      if ((!single_char)&&(scan_och==midch)&&(scan_ch==trmch)) return;
      scan_next_char();}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean test_if_char_is_first_on_line()
{
   Boolean register answer;
   char register *ppp;
   char holdch;
   /*-------------------------------------------------------*/
   if (sinfo.line_buffer_ix<2) return(answer=TRUE);
   ppp = sinfo.line_buffer+sinfo.line_buffer_ix-2;
   holdch = (*ppp);
   *ppp = '\0';
   answer = is_a_blank_line(sinfo.line_buffer);
   *ppp = holdch;
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
void give_ch_info(short *curr_lno,short *curr_pos,char *curr,char *look)
{
   /*-------------------------------------------------------*/
   *curr_lno = sinfo.char_pair.current_ch_lno;
   *curr_pos = sinfo.char_pair.current_ch_pos;
   *curr = equiv_text_char((int) sinfo.char_pair.current_ch);
   *look = equiv_text_char((int) sinfo.char_pair.lookahead_ch);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void decide_bugfile()
{
   /*-------------------------------------------------------*/
   bugfile=listfile;
   if (bitset(debug_flagword,DBG_XPN)) bugfile=errorfile;
   if (bugfile==((FILE *) NULL)) bugfile=listfile;
   if (pipe_model_to_stdout){
      modfile=stdout;
      outputfile=stderr;}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void list_page_indented_line(short indent_cols,char *text)
{
   register short i;
   /*-------------------------------------------------------*/
   for (i=0;i<indent_cols;++i) fprintf(listfile," ");
   fprintf(listfile,"%s",text);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void new_list_page()
{
   short col,need;
   /*-------------------------------------------------------*/
   ++listpage;
   if ((listpage>1)||(form_feed_on_first)) fprintf(listfile,"%s",FORM_FEED);
   fprintf(listfile,"%s    ",input_filename);
   fprintf(listfile,"%s %s",date_string,time_string);
   col = 5 + strlen(input_filename) +
             strlen(date_string) + strlen(time_string) +
             strlen(brief_revmsg);
   need=10;
   if (listpage>9) ++need; if (listpage>99) ++need; if (listpage>999) ++need;
   need=87-col-need;
   if (need<4) need=4;
   for (col=0;col<need;++col) fprintf(listfile," ");
   fprintf(listfile,"%s    Page %d\n\n",brief_revmsg,(int) listpage);
   listline=3;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void list_page_need_test_only(short knt)
{
   /*-------------------------------------------------------*/
   if (listline+knt>list_lines_per_page) new_list_page();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void list_page_need(short knt)
{
   /*-------------------------------------------------------*/
   if (listline+knt>list_lines_per_page) new_list_page();
   listline += knt;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void new_map_section(char *section_name,void *ppp)
{
   static short min_line_knt=6;
   /*-------------------------------------------------------*/
   list_page_need_test_only(min_line_knt);
   listline += 2;
   fprintf(listfile,"\n%s",section_name);
   if (ppp!=((void *) NULL)){
      fprintf(listfile," %s",STRLB);
      fprintf_hex_pointer(listfile,ppp);
      fprintf(listfile,STRRB);}
   fprintf(listfile,":\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fprintf_map_memix(FILE *fp,char *ppp)
{
   Memix diff;
   /*-------------------------------------------------------*/
   if (ppp == ((char *) NULL))
      fprintf(fp,NULL_MSG);
   else{
      diff = ppp - map_ppp0;
      fprintf_hex_memix(fp,diff);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void map_page_addr(char *ppp)
{
   Memix diff;
   /*-------------------------------------------------------*/
   fprintf(listfile,"    ");
   if (ppp == ((char *) NULL))
      fprintf(listfile,NULL_MSG);
   else{
      diff = ppp - map_ppp0;
      fprintf_hex_memix(listfile,diff);}
   fprintf(listfile,":   ");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void enk_line_and_pos(char *str,char delimch)
{
   /*-------------------------------------------------------*/
   sprintf(str,"%c%d%c%d%c",
               delimch,(int) sinfo.line_number,
               delimch,(int) arrow_position,
               delimch);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_value_irange(state_offset_type *offinfo)
{
   /*-------------------------------------------------------*/
   parse_irange(offinfo,I_RANGE_MAX);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_subscript_irange(state_offset_type *offinfo)
{
   /*-------------------------------------------------------*/
   parse_irange(offinfo,255);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void modfile_2_objectfile()
{
   long pos,iii;
   int ich;
   /*-------------------------------------------------------*/
   pos=ftell(modfile);
   if (pos>0){
      rewind(modfile);
      store_table_front(OBJ_VERBATIM_HEAD,1,(size_t) pos);
      for (iii=0;iii<pos;++iii){
         ich = fgetc(modfile);
         fputc(ich,objectfile);}
      fseek(modfile,pos,SEEK_SET);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
