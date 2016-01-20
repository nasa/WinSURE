#if !defined(_ZZ_q_astr_ext_DEFINED)
#define _ZZ_q_astr_ext_DEFINED
/*===========================================================================*/
#include "commonio.h"
#include "cm_types.h"
/*===========================================================================*/
extern Boolean pack_state_space_value_err
       (ssvar_value_type,state_bitstring_type,state_offset_type *);
extern void set_bit(unsigned char *,bitsize_type);
extern unsigned char clear_byte_tail(unsigned char *,bitsize_type);
extern Boolean test_bit(unsigned char *,bitsize_type);
/*===========================================================================*/
#endif
