#if !defined(_ZZ_q_cm_prsvars_DEFINED)
#define _ZZ_q_cm_prsvars_DEFINED
/*===========================================================================*/

#include "lib_def.h"

#include "cm_types.h"

EXKW char unbracketed_err_word[6];   /* "ERROR" or "ERR" */
EXKW char unbracketed_wrn_word[10];  /* "WARNING" or "WRN" */
EXKW char unbracketed_svr_word[10];  /* "SEVERITY" or "SEV" */
EXKW char warning_spacing_char;
EXKW short nesting_scope_level;
EXKW debug_flag_type debug_flagword;
EXKW char scratch_string_buffer[STRING_MAXNCH_P];
EXKW char os_malloc_limit_msg[60];
EXKW int_type eval_sub_infix_expr_ans[3];
EXKW short ee_oix_out,ee_ix_out;
EXKW value_union_type ee_aux_val;

                 /************************************************/
                 /************************************************/
                 /****                                        ****/
                 /****   USER ERROR POINTER/INDEX VARIABLES   ****/
                 /****                                        ****/
                 /************************************************/
                 /************************************************/

#ifdef STORAGE
EXKW Boolean skipping_inside_comment=FALSE;
EXKW short arrow_position=0;         /* pointer to error in source languages */
#else
EXKW Boolean skipping_inside_comment;/* true only in comment skipping mode */
EXKW short arrow_position;           /* pointer to error in source languages */
#endif
                      /***************************************/
                      /***************************************/
                      /****                               ****/
                      /****   USER MAINTAINED VARIABLES   ****/
                      /****                               ****/
                      /***************************************/
                      /***************************************/

extern char language[];              /* user supplied (name of program) */
extern short error_count;            /* cumulative (user error routine) */
extern Boolean end_of_file_reached;  /* user readch routine maintains this */
extern Boolean capise_mode;          /* flag to capitalize all identifiers */
extern short max_input_line_length;  /* maximum number of chars/line input */
extern short max_output_line_length; /* maximum number of chars/line */
extern short half_output_line_length;/* max_output_line_length/2 */

/*===========================================================================*/
#endif
