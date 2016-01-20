#if !defined(_ZZ_q_fix_ext_DEFINED)
#define _ZZ_q_fix_ext_DEFINED
/*===========================================================================*/
#include "prstypes.h"
/*===========================================================================*/
extern void set_map_addr0(char *);
extern void fixup_an_expr(expression_type *);
extern void *fixup_pointer(relative_address_type,short);
extern void fixup_identifier_table(long);
extern void fixup_expressions(void);
extern void fixup_user(void);
/**/
#define FIXUP(what,ooo) fixup_pointer((relative_address_type) what,(short) ooo)
#define INST_FIXUP(what,ooo) what=(instruction_type *) FIXUP(what,ooo)
#define Set_ooostore() \
        ooostore = ooo; \
        if (bitset((*idinfo).flags,VARIABLE_TYPE)) \
            ooostore += COMPUTATIONAL_TYPE_COUNT
/*===========================================================================*/
#endif
