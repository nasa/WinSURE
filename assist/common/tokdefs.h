                         /********************/
                         /********************/
                         /****            ****/
                         /****   TOKENS   ****/
                         /****            ****/
                         /********************/
                         /********************/

#if !defined(_ZZ_q_tokdefs_DEFINED)
#define _ZZ_q_tokdefs_DEFINED
/*===========================================================================*/

#define TK_NULL    ((token) 0)
/*------------------------------  identifier tokens (must be in sequence) */
#define TK_WITH_VALUE_0 TK_ID
#define TK_ID      ((token) 1)
#define TK_MACPARM ((token) 2)
/*------------------------------  literal tokens (must be in sequence) */
#define TK_LITERAL_0 TK_BOOL  /* see also: prstabsP.h */
#define TK_BOOL    ((token) 3)
#define TK_CHAR    ((token) 4)
#define TK_INT     ((token) 5)
#define TK_REAL    ((token) 6)
#define TK_LITERAL_9 TK_REAL
#define TK_WITH_VALUE_9 TK_REAL
/*------------------------------  relation tokens (must be in sequence) */
#define TK_RELATION_0 TK_EQ  /* see also: prstabsP.h */
#define TK_EQ      ((token) 10)  /* =  */
#define TK_NE      ((token) 11)  /* <> */
#define TK_LT      ((token) 12)  /* <  */
#define TK_GT      ((token) 13)  /* >  */
#define TK_LE      ((token) 14)  /* <= */
#define TK_GE      ((token) 15)  /* >= */
#define TK_RELATION_9 TK_GE
#define TK_EQ_REL_0 TK_EQ
#define TK_EQ_REL_9 TK_NE
#define TK_INEQ_REL_0 TK_LT
#define TK_INEQ_REL_9 TK_GE
#define TK_BOOL_EQ (token) 16 /* == */
#define TK_BOOL_NE (token) 17 /* ~~ */
/*------------------------------  operation tokens */
#define TK_OPERATION_0 TK_NOT  /* see also: prstabsP.h */
#define TK_NOT     ((token) 30)  /* ~ */
#define TK_AND     ((token) 31)  /* & */
#define TK_OR      ((token) 32)  /* | */
#define TK_XOR     ((token) 33)  /* XOR */
#define TK_NEG     ((token) 34)  /* - (unary) */
#define TK_ARITH_0 TK_INC_1  /*-------------------*/
#define TK_INC_1   ((token) 35)  /* -- */
#define TK_DEC_1   ((token) 36)  /* ++ */
#define TK_ADD     ((token) 37)  /* + */
#define TK_SUB     ((token) 38)  /* - (binary) */
#define TK_MUL     ((token) 39)  /* * */
#define TK_DVD     ((token) 40)  /* / */
#define TK_MOD     ((token) 41)  /* % */
#define TK_CYC     ((token) 42)  /* CYC */
#define TK_QUO     ((token) 43)  /* DIV */
#define TK_POW     ((token) 44)  /* ** */
#define TK_ARITH_9 TK_POW    /*-------------------*/
#define TK_CONCAT  (token) 45  /* ^ */
#define TK_OPERATION_9 TK_CONCAT
/*------------------------------  misc punctuation tokens */
#define TK_COMMA   ((token) 70)  /* , */
#define TK_SEMI    ((token) 71)  /* ; */
#define TK_LP      ((token) 73)  /* left paren */
#define TK_RP      ((token) 75)  /* right paren */
#define TK_LB      ((token) 76)  /* left bracket */
#define TK_RB      ((token) 77)  /* right bracket */
#define TK_AT      ((token) 78)  /* @ */
#define TK_COLON   ((token) 79)  /* : */
#define TK_SUBRNGE ((token) 80)  /* .. */
/*------------------------------  reserved word token */
#define TK_RW      ((token) 253)
#define TK_UNUSED  ((token) 254)
#define TK_EOF     ((token) 255)

/*===========================================================================*/
#endif
