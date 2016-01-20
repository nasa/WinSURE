#if !defined(_ZZ_q_io_ext_DEFINED)
#define _ZZ_q_io_ext_DEFINED
/*===========================================================================*/

          /******************************************************/
          /******************************************************/
          /****                                              ****/
          /****   PROTOTYPES FOR THE LOW-LEVEL I/O LIBRARY   ****/
          /****                                              ****/
          /******************************************************/
          /******************************************************/

#include "cm_sys.h"
#include "cm_types.h"
#include "user_ext.h"

#include "ast_lexio_vars.h"

extern void init_iolib(void);
extern void init_sinf(void);
extern void re_init_iolib(void);
extern short strip_command_line_value(char *,char *);
extern void default_command_line_options(void);
extern char *command_line_positional_arg(int,char **,int);
extern void parse_command_line_options(int,char **);
#ifdef BUGGY
extern void set_debug_options(void);
extern void dump_all_error_messages(void);
#endif
extern void compute_dependent_options(void);
extern void echo_inserted_init(void);
extern void echo_inserted_force_newline(void);
extern void echo_inserted_newarrow(short);
extern short set_echo_inserted_indent(short);
extern short echo_inserted_newline(short);
extern void echo_inserted_newline_final(short);
extern void echo_inserted_newline_indent(short);
extern void echo_inserted_token_to_listing(char *);
extern void print_listing_line(short,char *,FILE *);
extern short siglen_wrapping_message(FILE *,char *,short,short,short,short);
extern short wrapping_message(FILE *,char *,short,short,short,short);
extern void show_err_or_wrn(FILE *,short,char *,void *,type_flagword_type);
extern void error_bump(void);
extern void warning_bump(short);
extern int_type show_int_err(short,int_type);
extern real_type show_real_err(short,real_type);
extern void show_err(FILE *,short,void *,type_flagword_type);
extern void show_wrn(FILE *,short,void *,type_flagword_type,short);
extern void wrapping_message_on_both(char *);
extern void show_with_fmt_on_both(char *,short,char *);
extern void show_error_counts_on_both(void);
extern void save_old_line(void);
extern Boolean buffered_load_line(FILE *);
extern int buffered_getc(FILE *);
extern char readch(char);
extern void switch_to_stdin(char *,short);
extern void switch_back_to_inputfile(short);
extern char readch_and_cleanup(char);
extern void both_printf(char *s);
extern void prompt_for_prefix_if_missing(char *,char *,char *);
extern void tok_after_matching_right_paren(char *,Boolean);
extern void scan_next_char(void);
extern Boolean scan_skip_if_comment(void);
extern Boolean scan_skip_if_special(void);
extern void scan_to_matching(char);
extern void finish_scan_to_matching(char);
extern void scan_over_comment(char,char);
extern Boolean test_if_char_is_first_on_line(void);
extern void decide_bugfile(void);
extern void list_page_indented_line(short,char *);
extern void new_list_page(void);
extern void list_page_need_test_only(short);
extern void list_page_need(short);
extern void new_map_section(char *,void *);
extern void fprintf_map_memix(FILE *,char *);
#define fshew_map_memix(fp,ptr) fprintf_map_memix(fp,(char *) ptr)
extern void map_page_addr(char *);
extern void modfile_2_objectfile(void);
/*------------------------------------------------------  user supplied */
extern void expression_map_sections(void);
extern void application_map_sections(void);
extern void parse_value_irange(state_offset_type *);
extern void parse_subscript_irange(state_offset_type *);
/*===========================================================================*/

#include "ast_lexio_vars.h"

#define give_short(big,ee) (short) \
        (((((long) (big))>((long) MAXSHORT)) || \
          (((long) (big))<((long) (-(MAXSHORT))))) \
            ? show_int_err(ee,(int_type) (big)) \
            : (big))
#define give_Subscript(big,ee) give_short(big,ee)
/*
   determine if word wrapping disallowed after character "c".
*/
#define is_text_nowrap_after_char(c) \
        ( \
           ((c)==OPENPAREN) || ((c)==QUOTATION_MARK) || ((c)==APOSTROPHE) || \
           ((c)=='<') || ((c)=='_') || ((c)=='-') || ((c)=='$') || \
           (!(ispunct(c)||isspace(c)||((c)=='\0'))) \
        )
/*===========================================================================*/
#endif
