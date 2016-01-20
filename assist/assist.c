/*
===============================================================================
********************************************************************************************
Notices:


Copyright 2001 United States Government as represented by the Administrator of the National 
Aeronautics and Space Administration. 
All Rights Reserved.    

Disclaimers


No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, 
EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT
THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT 
SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO 
THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY 
GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE 
PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, 
GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF 
PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."

Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES 
GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  
IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES
OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, 
RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED 
STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE 
EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, 
UNILATERAL TERMINATION OF THIS AGREEMENT.
*********************************************************************************************

        The ASSIST main program.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)
              3/92   Changed release number to 7.1 to reflect changes
                     required by TOTAL.
            3/25/92  Added code to parse a ".assist_options" file.  This
                     is useful in conjunction with TOTAL.
            10/7/99  Fix bug shown by dan.ast --> 7.2

===============================================================================
*/

#include "main_def.h"

#include "common.h"

#include "objdefs.h"
#include "objtypes.h"
#include "objvars.h"

#include "macvars.h"

#include "astdefs.h"
#include "asttypes.h"
#include "astvars.h"

#include "clinelib_ext.h"
#include "astr_ext.h"
#include "io_ext.h"
#include "alex_ext.h"
#include "obj_ext.h"
#include "mac_ext.h"
#include "aprs_ext.h"
#include "agen_ext.h"

#include "alextabs.h"
#include "astoptfig.h"


char full_revmsg[] =
"ASSIST VERSION 7.4                         NASA Langley Research Center";
char brief_revmsg[]="ASSIST 7.4, NASA LaRC";


extern int main(int,char **,char **);

int main(int argc,char **argv,char **envp)
{
   static char prompt[] = "Enter ASSIST input file";
   static char run_exit[] = "\nRUN;\nEXIT;\n";
   static char autorun_exit[] = "\nRUN %s.dat;\nEXIT;\n";
   char cline[82];
   char *posarg;
   /*-------------------------------------------------------*/
   fprintf(stderr,"%s\n",full_revmsg);
   /*------------------------------------  init system/string libraries */
   //fprintf(stderr,"$$$$$$$$$$$$$$$$$$ MAX_INT_TYPE = %i \n",MAX_INT_TYPE);
   init_syslib();
   init_strlib();
   /*------------------------------------  parse command line */
   posarg=command_line_positional_arg(argc,argv,1);
   prompt_for_prefix_if_missing(prompt,".ast",posarg);
   append_suffix_to_filename(".ast",input_filename);
   append_suffix_to_filename(".mod",mod_filename);
   append_suffix_to_filename(".alog",list_filename);
   append_suffix_to_filename(".aob",object_filename);
   append_suffix_to_filename(".aex",exe_filename);
   /*------------------------------------  initialize other libraries */
   init_iolib();
   init_lexlib(reserved_word_lookup_table);
   init_objlib();
   function_body_size_used=init_prslib();
   /*------------------------------------  */
   default_command_line_options();
   set_user_home_directory(envp,OPTIONS_CONFIG_FILE);
   parse_command_line_options(argc,argv);
   wrn_err_auto_msg_sizing=TRUE;
   /*------------------------------------  */
   inputfile=open_input_using_big_buffer(input_filename);
   listfile=open_output_file(list_filename);
   modfile=stdout;
   outputfile=stdout;
   errorfile=stderr;
   objectfile=open_binary_output_file(object_filename);
   make_temporary_filename(xref_filename);
   xreffile=open_binary_output_file(xref_filename);
   /*------------------------------------  */
#ifdef BUGGY
set_debug_options();
debug_flagword=DBG_ALL;
#endif
   /*begin*/
      re_init_syslib();
      re_init_strlib();
      re_init_iolib();
      re_init_lexlib();
      re_init_objlib();
      re_init_prslib(&function_body_count,&macro_stack_top);
      compute_dependent_options();
      must_assist_alloc_for_parse=TRUE;
      advance_one_token();
      parse_program();
      store_identifier_table();
      assist_store_space_and_misc();
      modfile_2_objectfile();
      if (autorun){
         sprintf(cline,autorun_exit,filename_prefix);
         store_in_object_file(OBJ_VERBATIM_TAIL,
                              (void *) cline,
                              strlen(cline),1);}
      else if (pipe_model_to_stdout){
         store_in_object_file(OBJ_VERBATIM_TAIL,
                              (void *) run_exit,strlen(run_exit),1);}
      rewrite_object_header();
      fclose(objectfile); objectfile=NULL;
      do_xref_map();
      fclose(xreffile); xreffile=NULL;
      cleanup_temporary_filename(xref_filename);
      if (fatal_error){
         remove(object_filename);
         abort_program_quietly();}
      assist_free_after_parse();
   /*end*/
   free_big_input_buffer();
   parsing_input_file=FALSE;
#ifdef BUGGY
   fprintf(outputfile,"freeing memory...\n");
#endif
   load_obj_file_allocating_memory(TRUE);
   if (!no_statistics) show_elapsed_processing_time();
   init_elapsed_time();
   if (fatal_error) abort_program_quietly();
   fflush(listfile);
   init_hash_function();
   if (!no_statistics) fprintf(outputfile,"generating SURE model file...\n");
   if (!no_generate) generate_model();
   if (!no_statistics){
      show_elapsed_generation_time();
      show_state_counts();}
#ifdef BUGGY
fprintf(bugfile,"To show error counts ::: ");
showtoken(this.tok,this.rw,this.id);
#endif
   show_error_counts_on_both();
   if (!no_statistics){
      if (force_statistics ||
          bitset(debug_flagword,DBG_XEQ)) show_bucket_statistics();}
   if (sure_tail_buffer != ((char *) NULL)){
      fprintf(modfile,"%s",sure_tail_buffer);
      g_free(sure_tail_buffer);}
   /*-------------------------------------------------------*/
   exit(0);
   return(0);
}
