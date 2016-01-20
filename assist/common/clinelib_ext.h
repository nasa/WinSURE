#if !defined(_ZZ_q_clinelib_ext_DEFINED)
#define _ZZ_q_clinelib_ext_DEFINED
/*===========================================================================*/
#include "commonio.h"
/*===========================================================================*/
#define ARG_MAXLEN 80
#define ARG_MAXLEN_P 81
/*===========================================================================*/
#if defined(WITH_TAE_DCLS) || (!defined(_ZZ_q_cm_sys_DEFINED))
extern char user_home_dir[];
extern char user_config_file[];
#else
EXKW pathname_type user_home_dir;
EXKW pathname_type user_config_file;
#endif
/*===========================================================================*/
#ifdef _NO_PROTO
extern flagtyp process_redirection_string();
extern void do_shew_parse_command_line_opts();
extern void freeup_from_parse_options_file();
extern short parse_options_file();
extern void set_user_home_directory();
#else
extern flagtyp process_redirection_string(char *);
extern void do_shew_parse_command_line_opts(int,int,char **);
extern void freeup_from_parse_options_file(void);
extern short parse_options_file(char *,char ***);
extern void set_user_home_directory(char **,char *);
#endif
/*===========================================================================*/
#endif
