#if !defined(_ZZ_q_eval_ext_DEFINED)
#define _ZZ_q_eval_ext_DEFINED
/*===========================================================================*/

#include "commonio.h"
#include "safeeval.h"
#include "cm_types.h"
#include "prstypes.h"

#ifdef _NO_PROTO
extern void init_prseval();
extern type_flagword_type evaluate_expression();
extern type_flagword_type fast_evaluate_full_expression();
#else
extern void init_prseval(void);
extern type_flagword_type evaluate_expression
      (expression_type *,value_union_type *,Boolean,short,short,short,Boolean);
extern type_flagword_type fast_evaluate_full_expression
      (expression_type *,value_union_type *);
#endif

/*===========================================================================*/
#endif
