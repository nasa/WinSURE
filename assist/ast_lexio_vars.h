#if !defined(_ZZ_q_ast_lexio_vars_DEFINED)
#define _ZZ_q_ast_lexio_vars_DEFINED
/*===========================================================================*/

#include "lib_def.h"
#include "cm_types.h"
#include "ast_lexio_defs.h"

#ifdef ASSIST
EXKW state_bitstring_type this_state_bits;
#endif

EXKW Boolean parsing_input_file,executing_rules,end_of_file_reached;
EXKW char *sure_declarations;
EXKW char *sure_tail_buffer;
EXKW char *allvarstorage;
EXKW char *allvarstorage_copy;
EXKW size_t allvarstorage_size;
EXKW char *map_ppp0;


                        /**********************************/
                        /**********************************/
                        /****                          ****/
                        /****   COMMAND LINE OPTIONS   ****/
                        /****                          ****/
                        /**********************************/
                        /**********************************/

/*-------------------------------------------    true command line options */
EXKW Boolean pipe_model_to_stdout; /* -PIPE (pipe model) */
EXKW Boolean no_generate;          /* -NOGEN (no rule generation) */
EXKW Boolean no_statistics;        /* -NOSTATS (no statistics) */
EXKW Boolean memory_map;           /* -LOADMAP (memory map) */
EXKW Boolean xref_map;             /* -XREF (cross reference map) */
EXKW Boolean capise_mode;          /* -C (case sensitive) */
EXKW Memix ReallocBucketsAtTime;   /* -BI=# (bucket incrment) */
EXKW Memix max_spacepix_count;     /* -PIC=# (state-space picture limit) */
EXKW Memix max_idn_count;          /* -I=# (ident table limit) */
EXKW Memix max_num_count;          /* -N=# (number table limit) */
EXKW Memix max_opnd_count;         /* -O=# (operand table limit) */
EXKW Memix max_expr_count;         /* -E=# (max num. of scratch expressions) */
EXKW Memix max_parm_count;         /* -P=# (function parameter count limit) */
EXKW Memix max_nest_count;         /* -NEST=# (function nesting limit) */
EXKW Memix max_body_count;         /* -B=# (function body token count limit) */
EXKW Memix max_rule_count;         /* -RULE=# (rule count limit) */
EXKW Memix bucket_count;           /* -BC=# (bucket count) */
EXKW Memix bucket_width;           /* -BW=# (bucket width) */
EXKW Boolean multi_echo_on_error;  /* -ME (multi_echo) */
EXKW short max_input_line_length;  /* -WID=# (width of source code line) */
EXKW short max_output_line_length; /* future option */
EXKW short levels_of_warnings;     /* -W=# (num of warning levels requested) */
EXKW short max_errs_each_line;     /* -LEL=# (error limit) */
EXKW short max_wrns_each_line;     /* -LWL=# (warning limit) */
EXKW short max_errs;               /* -EL=# (error limit) */
EXKW short max_wrns;               /* -WL=# (warning limit) */
EXKW short tab_delta;              /* -TAB=# (chars between tab stops) */
EXKW short list_lines_per_page;    /* -LP=# (listing lines/page) */
EXKW Boolean three_letter_bracketed_words; /* -WE3 (wrn/err/3-letter) */
EXKW Boolean wrn_err_auto_msg_sizing;   /* -WEAUTO (wrn/err auto-sizing) */
EXKW Boolean simplify_rate_expressions; /* -SRE (simplify rate expressions) */
EXKW Boolean autorun;              /* -AUTORUN (-AUTO) (autorun) */
EXKW Boolean batch_mode;           /* -BATCH (-BAT) (batch mode) */
EXKW Boolean wrap_long_constant_expressions;
             /* -[NO_]EXPRESSION_TERM_WRAPPING (-[NO_]XTW) */
EXKW flagtyp debug_mode;
EXKW flagtyp gen_suppress;
EXKW flagtyp force_statistics;
/*-------------------------------------------    computed from cmd line opts */
EXKW Memix extension_bucket_count;
EXKW short list_line_width;
EXKW Memix max_ops_count;                /* 20 + 2 * -O */
EXKW Boolean mask_out_non_printing;      /* some future option maybe */
EXKW short half_output_line_length;      /* max_output_line_length/2 */
EXKW Memix max_identifier_table_alloc;   /* sum of -I,-N,-S */
EXKW Memix max_identifier_table_count;   /* sum of -I,-N */
EXKW Memix current_temporary_count;

                      /*************************************/
                      /*************************************/
                      /****                             ****/
                      /****   OPTION DEFINITION VALUES  ****/
                      /****                             ****/
                      /*************************************/
                      /*************************************/

EXKW struct qqoptdefs
    {
#ifdef ASSIST
         unsigned_31_type onedeath;
         unsigned_31_type trim;
         unsigned_31_type comment;
         unsigned_31_type comment_stats;
#endif
         unsigned_31_type echo;
    } option_rec;

                          /*******************************/
                          /*******************************/
                          /****                       ****/
                          /****   ERROR INFORMATION   ****/
                          /****                       ****/
                          /*******************************/
                          /*******************************/

EXKW Boolean program_currently_aborting;
EXKW short error_count;
EXKW short warning_count;
EXKW short highest_severity_encountered;
EXKW Boolean fatal_error;
EXKW short err_or_warn_last_indented0;

                   /********************************************/
                   /********************************************/
                   /****                                    ****/
                   /****   LANGUAGE DEPENDENT INFORMATION   ****/
                   /****                                    ****/
                   /********************************************/
                   /********************************************/

/*-------------------------------------*/
#ifdef STORAGE

#ifdef ASSIST
#define QQ_LANG "ASSIST"
#define QQ_LANG_LC "assist"
#define QQ_INTERACTIVE FALSE
#endif

#ifdef SURE
#define QQ_LANG "SURE"
#define QQ_LANG_LC "sure"
#define QQ_INTERACTIVE TRUE
#endif

#ifdef STEM
#define QQ_LANG "STEM"
#define QQ_LANG_LC "stem"
#define QQ_INTERACTIVE TRUE
#endif

#ifdef PAWS
#define QQ_LANG "PAWS"
#define QQ_LANG_LC "paws"
#define QQ_INTERACTIVE TRUE
#endif

#endif
/*-------------------------------------*/
#ifdef STORAGE
EXKW char language[] = QQ_LANG;
EXKW char language_command[] = QQ_LANG_LC;
EXKW Boolean interactive_language = QQ_INTERACTIVE;
#else
EXKW char language[];
EXKW char language_command[];
EXKW Boolean interactive_language;
#endif
/*===========================================================================*/
#endif
