
                   /*********************************/
                   /*********************************/
                   /****                         ****/
                   /****   RESERVED WORD TYPES   ****/
                   /****                         ****/
                   /****                         ****/
                   /****  PREDEFINED  CONSTANTS  ****/
                   /****           AND           ****/
                   /****  DESCRIPTIVE OPERATORS  ****/
                   /****       ARE OMITTED       ****/
                   /****                         ****/
                   /*********************************/
                   /*********************************/

#if !defined(_ZZ_q_rwdefs_DEFINED)
#define _ZZ_q_rwdefs_DEFINED
/*===========================================================================*/
/*
     Note:  Reserved words are partitioned as follows:

                  0-127  reserved for user applications
                128-255  reserved for common library/expression parsing

*/
/*--------------------------------------- names of built-in functions */
#define RW_BUILTIN_0 RW_SQRT
#define RW_SQRT       ((rwtype) 130)
#define RW_EXP        ((rwtype) 131)
#define RW_LN         ((rwtype) 132)
#define RW_ABS        ((rwtype) 133)
#define RW_SIN        ((rwtype) 134)
#define RW_COS        ((rwtype) 135)
#define RW_TAN        ((rwtype) 136)
#define RW_ARCSIN     ((rwtype) 137)
#define RW_ARCCOS     ((rwtype) 138)
#define RW_ARCTAN     ((rwtype) 139)
#define RW_FACT       ((rwtype) 140)
#define RW_GAM        ((rwtype) 141)
#define RW_COMB       ((rwtype) 142)
#define RW_PERM       ((rwtype) 143)
#define RW_SIZE       ((rwtype) 144)
#define RW_VARIAB_0 RW_COUNT
#define RW_COUNT      ((rwtype) 145)
#define RW_MIN        ((rwtype) 146)
#define RW_MAX        ((rwtype) 147)
#define RW_SUM        ((rwtype) 148)
#define RW_ANY        ((rwtype) 149)
#define RW_ALL        ((rwtype) 150)
#define RW_VARIAB_9 RW_ALL
#ifdef FUTURE
#define RW_ROWSUM     ((rwtype) 149)
#define RW_COLSUM     ((rwtype) 150)
#define RW_ROWCOUNT   ((rwtype) 151)
#define RW_COLCOUNT   ((rwtype) 152)
#define RW_ANYPOS     ((rwtype) 153)
#define RW_ALLPOS     ((rwtype) 154)
#define RW_OTHER      ((rwtype) 155)
#endif
#define RW_BUILTIN_9 RW_ALL
/*--------------------------------------- names of arithmetic keywords */
#define RW_NOT        ((rwtype) 170)
#define RW_AND        ((rwtype) 171)
#define RW_OR         ((rwtype) 172)
#define RW_XOR        ((rwtype) 173)
#define RW_QUO        ((rwtype) 174)
#define RW_MOD        ((rwtype) 175)
#define RW_CYC        ((rwtype) 176)
/*--------------------------------------- common keywords */
#define RW_OF         ((rwtype) 180)
#define RW_WITH       ((rwtype) 181)
#define RW_IN         ((rwtype) 182)
#define RW_ARRAY      ((rwtype) 183)
#define RW_INTEGER    ((rwtype) 184)
#define RW_REAL       ((rwtype) 185)
#define RW_BOOLEAN    ((rwtype) 186)
/*--------------------------------------- option definition keywords */
#define RW_OFF        ((rwtype) 190)
#define RW_ON         ((rwtype) 191)
#define RW_FULL       ((rwtype) 192)
/*--------------------------------------- */
/*
    The RW_DBG_??? flags must occur in the same order as listed in
    the definitions in DEFS.H without any holes.
*/
#define RW_DBG_NONE   ((rwtype) 200)
#define RW_DBG_LEX    ((rwtype) 201)
#define RW_DBG_PRS    ((rwtype) 202)
#define RW_DBG_TRANTO ((rwtype) 203)
#define RW_DBG_XEQ    ((rwtype) 204)
#define RW_DBG_ROU    ((rwtype) 205)
#define RW_DBG_DATAST ((rwtype) 206)
#define RW_DBG_EXPAND ((rwtype) 207)
#define RW_DBG_CODE   ((rwtype) 208)
#define RW_DBG_ABORT  ((rwtype) 214)
#define RW_DBG_ALL    ((rwtype) 215)
/*--------------------------------------- */
#define RW_UNUSED         ((rwtype) 251)
#define RW_NULL           ((rwtype) 252)
#define NOT_RESERVED_WORD ((rwtype) 255)
/*--------------------------------------- */

/*===========================================================================*/
#endif
