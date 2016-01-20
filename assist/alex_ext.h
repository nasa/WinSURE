#if !defined(_ZZ_q_alex_ext_DEFINED)
#define _ZZ_q_alex_ext_DEFINED
/*===========================================================================*/

#include "cm_sys.h"
#include "cm_types.h"
#include "user_ext.h"

#include "ast_lexio_vars.h"

#define init_astlex() re_init_astlex()
extern void re_init_astlex(void);
extern void store_identifier_table(void);
extern void do_xref_map(void);
extern int cmp_xref_entries(void *,void *);
extern void add_xref_item(char *,char);
#if !defined(PRODUCTION)
extern void shew_logical_rw(rwtype);
extern void shew_logical_token(token);
#endif
extern void pnt_pnt_id_tab_msg(FILE *,Memix,identifier_info_type *,Boolean);
extern short print_idinfo(FILE *,identifier_info_type *,int,int);
extern void print_identifier_table(FILE *,Boolean);
extern void show_rule_info_after_error
       (identifier_info_type *,int_type,int_type,void *,char *,short,
        state_bitstring_type);
extern token gettoken(rwtype *,char *,short *,short *);
/*===========================================================================*/
#endif
