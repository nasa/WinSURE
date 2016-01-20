#if !defined(_ZZ_q_cm_types_DEFINED)
#define _ZZ_q_cm_types_DEFINED
/*===========================================================================*/

#if !defined(WITH_TAE_DCLS)
#include "cm_sys.h"
#endif
#include "cm_defs.h"

typedef unsigned long relative_address_type;
typedef unsigned short bitsize_type;

typedef unsigned char Utiny;

#if !defined(WITH_TAE_DCLS)
typedef unsigned_255_type opcode_type;
typedef unsigned_255_type token;              /* 0..255 */
typedef unsigned_255_type rwtype;             /* 0..255 */
typedef unsigned_255_type mode_flag_type;     /* 0..255 */
#endif

typedef unsigned char *state_bitstring_type;

typedef short Subscript;

#if !defined(WITH_TAE_DCLS)
typedef unsigned_255_type type_flagword_type;       /* 0..255 */
typedef unsigned_255_type debug_flag_type;          /* 0..255 */
typedef unsigned_255_type ident_usage_flag_type;    /* 0..255 */
#endif

typedef struct t__scanning_character_info
   {
        short current_ch_lno;
        short lookahead_ch_lno;
        short current_ch_pos;
        short lookahead_ch_pos;
        char current_ch;
        char lookahead_ch;
   } scanning_character_info_type;

#if !defined(WITH_TAE_DCLS)
typedef short ssvar_value_type;
typedef struct t__state_offset
   {                               /* (state-space variable SSVAR_TYPE) */
      ssvar_value_type minval;     /* (state-space variable SSVAR_TYPE) */
      ssvar_value_type maxval;     /* (state-space variable SSVAR_TYPE) */
      bitsize_type bit_offset;     /* (state-space variable SSVAR_TYPE) */
      bitsize_type bit_length;     /* (state-space variable SSVAR_TYPE) */
   } state_offset_type;
#endif

#if defined(INT_32_BIT) 
#define FAST_STATE_OFFSET_REGS() register short *pppsh0,*pppsh1
#define FAST_STATE_OFFSET_COPY(dest,src) \
     pppsh0 = (short *) (dest); pppsh1 = (short *) (src); \
     *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; \
     *pppsh0++ = *pppsh1++; *pppsh0 = *pppsh1
#else
#define FAST_STATE_OFFSET_REGS()
#define FAST_STATE_OFFSET_COPY(dest,src) \
        sys_memcpy((char *) (dest),(char *) (src),sizeof(state_offset_type))
#endif

/*===========================================================================*/
#if !defined(WITH_TAE_DCLS)
/*===========================================================================*/
typedef union t__pointer_union
   {
     relative_address_type relative_address;
     short parameter_count;
     void *vvv;               /* included for completeness */
     Boolean *bbb;            /* used when BOOL_TYPE */
     state_offset_type *sss;  /* used when SSVAR_TYPE */
     char *ccc;               /* used when CHAR_TYPE */
     int_type *iii;           /* used when INT_TYPE */
     real_type *rrr;          /* used when REAL_TYPE */
   } pointer_union_type;

typedef union t__value_union
   {
     Boolean bbb;            /* used when BOOL_TYPE */
     char ccc;               /* used when CHAR_TYPE */
     int_type iii;           /* used when INT_TYPE */
     real_type rrr;          /* used when REAL_TYPE */
     state_offset_type sss;  /* used when SSVAR_TYPE */
     pointer_union_type ptr; /* used when ARRAY_TYPE bit is set */
#if defined(INT_32_BIT) 
     struct qqiiis{
        int_type iiia;
        int_type iiib;} pair;
#endif
   } value_union_type;
#if defined(INT_32_BIT) 
#define val_union_cpy(dest,src) \
        ((*(dest)).pair.iiia = (*(src)).pair.iiia, \
         (*(dest)).pair.iiib = (*(src)).pair.iiib)
#else
#define val_union_cpy(dest,src) sys_memcpy(dest,src,sizeof(value_union_type))
#endif

#define OFFSIZE sizeof(state_offset_type)

typedef struct t__dim_pair
  {
    Subscript lower;/* lower dimension (ARRAY) body-index (FUNCTION/IMPLICIT)*/
    Subscript upper;/* upper dimension (ARRAY) body-index (FUNCTION/IMPLICIT)*/
  } dim_pair_type;
#define dim_pair_cpy(dest,src) \
        ((*(dest)).lower = (*(src)).lower, \
         (*(dest)).upper = (*(src)).upper)
#define dim_range(pair) (pair.upper-pair.lower+1)
#define dim_offset(ix,typ,pair) ((ix)-((typ) pair.lower))
#define simplify(pair) pair.upper=pair.lower=SIMPLE_IDENTIFIER
#define DECIDE_IS_ARRAY_IS_DOUBLE(qidifo) \
        is_array = bitset((*qidifo).flags,ARRAY_TYPE); \
        is_double=(Boolean) \
             (is_array && \
              ((*qidifo).index.dims.second.lower != SIMPLE_IDENTIFIER))
#define for_all_dims(iii,pair) for (iii=pair.lower;iii<=pair.upper;++iii)

typedef struct t__identifier_info
  {
     pointer_union_type ptr;    /* address in memory / function-parm-count */
     union
        {
           struct qqbothidinfqq
              {
                 dim_pair_type first;
                 dim_pair_type second;
              } dims;
           dim_pair_type body;
        } index;
     short scope_level;         /* scope level (negative iff. inactive) */
     char name[IDENT_MAXNCH_P]; /* identifier to search for */
     type_flagword_type flags;  /* type information */
  } identifier_info_type;

typedef struct t__token_lookup
   {
     char token[3];
     token value;
   } token_lookup_type;

typedef struct t__rw_operator_lookup
   {
      rwtype rwsrc;
      token tokdest;
   } rw_operator_lookup_type;

typedef struct t__reserved_word_lookup
   {
      char text[15];
      rwtype rw;
   } reserved_word_lookup_type;

typedef struct t__token_info
   {
      identifier_info_type *id_info_ptr;
      short linnum;
      short pos;
      token tok;
      rwtype rw;
      char id[IDENT_MAXNCH_P];
   } token_info_type;

typedef identifier_info_type *idinf_p_type;
#ifdef _NO_PROTO
typedef idinf_p_type (*any_searchid_routine)();
#else
typedef idinf_p_type (*any_searchid_routine)(char *,type_flagword_type,short);
#endif
/*===========================================================================*/
#endif
/*===========================================================================*/
#endif
