#if !defined(_ZZ_q_prsdefs_DEFINED)
#define _ZZ_q_prsdefs_DEFINED
/*===========================================================================*/

#define VARIAB_LENG (-99)
#define ARR_N_IX (-2)

#define VARIAB_MAX_ARGS 32

                /**********************************/
                /**********************************/
                /****                          ****/
                /****   EXPRESSION OPERATORS   ****/
                /****                          ****/
                /**********************************/
                /**********************************/

#define OP_DIFF_PRECEDENCE_MASK 0xf8
#define OP_SAME_PRECEDENCE_MASK 0x07
#define OP_LOWOP OP_OR
#define OP_MIN_PREC ((operation_type) 0x00)
/*------------------------------------- BINARY OPERATORS */
#define is_binary_arith(op) \
        (((op)>=OP_BINARITH_0)&&((op)<OP_BINARITH_9))
#define OP_BINARITH_0 OP_OR
#define OP_OR      ((operation_type) 0x10) /* precedence 0x10 */
#define OP_XOR     ((operation_type) 0x11)
#define OP_AND     ((operation_type) 0x18) /* precedence 0x18 */
#define OP_BOOL_EQ ((operation_type) 0x20) /* precedence 0x20 */
#define OP_BOOL_NE ((operation_type) 0x21)
#define OP_LT      ((operation_type) 0x28) /* precedence 0x28 */
#define OP_GT      ((operation_type) 0x29)
#define OP_LE      ((operation_type) 0x2a)
#define OP_GE      ((operation_type) 0x2b)
#define OP_EQ      ((operation_type) 0x2c)
#define OP_NE      ((operation_type) 0x2d)
#define OP_ADD     ((operation_type) 0x30) /* precedence 0x30 */
#define OP_SUB     ((operation_type) 0x31)
#define OP_MUL     ((operation_type) 0x38) /* precedence 0x38 */
#define OP_DVD     ((operation_type) 0x39)
#define OP_MOD     ((operation_type) 0x3a)
#define OP_CYC     ((operation_type) 0x3b)
#define OP_QUO     ((operation_type) 0x3c)
#define OP_POW     ((operation_type) 0x40) /* precedence 0x40 */
#define OP_RPOWR   ((operation_type) 0x41)
#define OP_IPOWI   ((operation_type) 0x42) 
#define OP_RPOWI   ((operation_type) 0x43)
#define OP_BINARITH_9 ((operation_type) 0x4f)
#define OP_IXDBY   ((operation_type) 0xf8) /* precedence 0xf8 */
#define OP_IXDBY2  ((operation_type) 0xf9) /* precedence 0xf8 */
#define OP_I_LB    ((operation_type) 0xfa) /* infix equivalent [ */
#define OP_I_WILD  ((operation_type) 0xfb) /* infix equivalent * */
#define OP_I_RB    ((operation_type) 0x02) /* infix equivalent ] (low prec.) */
/*------------------------------------- TERNARY OPERATORS */
#define is_ternary(op) ((op)==OP_IXDBY2)
/*------------------------------------- GROUPING OPERATORS */
#define OP_PARENS  ((operation_type) 0x50) /* precedence 0x50 */
#define OP_I_LP    ((operation_type) 0x51) /* infix equivalent ( */
#define OP_I_RP    ((operation_type) 0x01) /* infix equivalent ) (low prec.) */
#define OP_I_CMMA  ((operation_type) 0x03) /* infix separator  , (low prec.) */
/*------------------------------------- UNARY OPERATORS */
#define is_unary(op) \
        (((op)>=OP_INC)&&((op)<OP_BINARY_BUILTIN_0))
/*------------------------------------- RATE DESTINATION OPERATORS (UNARY) */
#define OP_INC     ((operation_type) 0x58) /* precedence 0x58 */
#define OP_DEC     ((operation_type) 0x59)
/*------------------------------------- UNARY OPERATORS */
#define OP_NOT     ((operation_type) 0x60) /* precedence 0x60 */
#define OP_NEG     ((operation_type) 0x68) /* precedence 0x68 */
#define OP_STNCHR  ((operation_type) 0x69)
#define OP_STNBOO  ((operation_type) 0x6a)
#define OP_STNINT  ((operation_type) 0x6b)
#define OP_STNRE   ((operation_type) 0x6c)
#define OP_ItoR    ((operation_type) 0x6d)
#define OP_BtoI    ((operation_type) 0x6e)
/*------------------------------------- BUILT-IN OPERATORS BEGIN */
#define is_builtin(op) \
        (((op)>=OP_BUILTIN_0)&&((op)<=OP_BUILTIN_9))
#define OP_BUILTIN_COUNT (OP_BUILTIN_9-OP_BUILTIN_0+1)
#define OP_BUILTIN_0 OP_SQRT
/*------------------------------------- UNARY BUILT-IN OPERATORS */
#define OP_SQRT     ((operation_type) 0x70) /* precedence 0x70 (built-ins) */
#define OP_EXP      ((operation_type) 0x71)
#define OP_LN       ((operation_type) 0x72)
#define OP_ABS      ((operation_type) 0x73)
#define OP_SIN      ((operation_type) 0x74)
#define OP_COS      ((operation_type) 0x75)
#define OP_TAN      ((operation_type) 0x76)
#define OP_ARCSIN   ((operation_type) 0x77)
#define OP_ARCCOS   ((operation_type) 0x78)
#define OP_ARCTAN   ((operation_type) 0x79)
#define OP_FACT     ((operation_type) 0x7a)
#define OP_GAM      ((operation_type) 0x7b)
#define OP_ONE_ARY_0 OP_SIZE
#define OP_SIZE     ((operation_type) 0x7f)
/**/
#define OP_IDENTITY_ARY_0 OP_COUNT1
#define OP_COUNT1   ((operation_type) 0x80)
#define OP_IMIN1    ((operation_type) 0x81)
#define OP_RMIN1    ((operation_type) 0x82)
#define OP_IMAX1    ((operation_type) 0x83)
#define OP_RMAX1    ((operation_type) 0x84)
#define OP_ISUM1    ((operation_type) 0x85)
#define OP_RSUM1    ((operation_type) 0x86)
#define OP_ANY1     ((operation_type) 0x87)
#define OP_ALL1     ((operation_type) 0x88)
#define OP_IDENTITY_ARY_9 OP_ALL1
#define OP_ONE_ARY_9 OP_ALL1
/**/
#define OP_ARRY_ARY_0 OP_COUNT
#define OP_COUNT    ((operation_type) 0x90)
#define OP_IMIN     ((operation_type) 0x91)
#define OP_RMIN     ((operation_type) 0x92)
#define OP_IMAX     ((operation_type) 0x93)
#define OP_RMAX     ((operation_type) 0x94)
#define OP_ISUM     ((operation_type) 0x95)
#define OP_RSUM     ((operation_type) 0x96)
#define OP_ANY      ((operation_type) 0x97)
#define OP_ALL      ((operation_type) 0x98)
#define OP_ARRY_ARY_9 OP_ALL
/**/
#define ARRAY_OP_COUNT ((operation_type) 16) /* OP_COUNT - OP_COUNT1 */
/**/
#define cvt_infix_varying_to_single(op) ((op) - ARRAY_OP_COUNT)
/*------------------------------------- BINARY BUILT-IN OPERATORS */
#define OP_BINARY_BUILTIN_0 OP_COMB
#define OP_COMB     ((operation_type) 0xa0)
#define OP_PERM     ((operation_type) 0xa1)
#define OP_ROWCOL_0 OP_ROWCOUNT
#define OP_ROWCOUNT ((operation_type) 0xb0)
#define OP_COLCOUNT ((operation_type) 0xb1)
#define OP_IROWMIN  ((operation_type) 0xb2)
#define OP_ICOLMIN  ((operation_type) 0xb3)
#define OP_RROWMIN  ((operation_type) 0xb4)
#define OP_RCOLMIN  ((operation_type) 0xb5)
#define OP_IROWMAX  ((operation_type) 0xb6)
#define OP_ICOLMAX  ((operation_type) 0xb7)
#define OP_RROWMAX  ((operation_type) 0xb8)
#define OP_RCOLMAX  ((operation_type) 0xb9)
#define OP_IROWSUM  ((operation_type) 0xba)
#define OP_ICOLSUM  ((operation_type) 0xbb)
#define OP_RROWSUM  ((operation_type) 0xbc)
#define OP_RCOLSUM  ((operation_type) 0xbd)
#define OP_ROWANY   ((operation_type) 0xbe)
#define OP_COLANY   ((operation_type) 0xbf)
#define OP_ROWALL   ((operation_type) 0xc0)
#define OP_COLALL   ((operation_type) 0xc1)
#define OP_ROWCOL_9 OP_COLALL
/*------------------------------------- VARIABLE LENG OPERATORS */
#define is_variab_leng_ary(op) \
        (((op)>=OP_VARIAB_LENG_BUILTIN_0)&&((op)<OP_CONCAT))
#define is_row_col_ary(op) \
        (((op)>=OP_ROWCOL_0)&&((op)<=OP_ROWCOL_9))
#define is_arry_ary(op) \
        (((op)>=OP_ARRY_ARY_0)&&((op)<=OP_ARRY_ARY_9))
#define is_one_ary(op) \
        (((op)>=OP_ONE_ARY_0)&&((op)<=OP_ONE_ARY_9))
#define is_identity_ary(op) \
        (((op)>=OP_IDENTITY_ARY_0)&&((op)<=OP_IDENTITY_ARY_9))
#define cvt_to_infix_if_builtin(op) \
        ((is_variab_leng_ary(op)) \
            ? ((op) - (OP_LISALL_OP_ALL_DIFF)) \
            : ((is_row_col_ary(op)) \
                  ? (OP_COUNT + (((op) - OP_ROWCOUNT)/2)) \
                  : (op)))
#define OP_LISALL_OP_ALL_DIFF ((operation_type) 0x40)
/*------------------------------------- VARIABLE BUILT-IN OPERATORS */
#define OP_VARIAB_LENG_BUILTIN_0 OP_LISCOUNT
#define OP_LISCOUNT ((operation_type) 0xd0)
#define OP_ILISMIN  ((operation_type) 0xd1)
#define OP_RLISMIN  ((operation_type) 0xd2)
#define OP_ILISMAX  ((operation_type) 0xd3)
#define OP_RLISMAX  ((operation_type) 0xd4)
#define OP_ILISSUM  ((operation_type) 0xd5)
#define OP_RLISSUM  ((operation_type) 0xd6)
#define OP_LISANY   ((operation_type) 0xd7)
#define OP_LISALL   ((operation_type) 0xd8)
#ifdef FUTURE
#define OP_OTHER    ((operation_type) 0xd9)
#endif
#define OP_BUILTIN_9 OP_LISALL
/**/
#define list_version_op(op) ((op) + (OP_LISALL_OP_ALL_DIFF))
#define rowcol_version_op(op,ix) \
        (OP_ROWCOUNT+((operation_type) ((ix)+2*((op)-OP_LISCOUNT))))
#define builtin_invalid_in_SURE(op) \
        (is_variab_leng_ary(op)||is_row_col_ary(op)|| \
         (((op)>=OP_ONE_ARY_0)&&((op)<=OP_ARRY_ARY_9)))
/*------------------------------------- BINARY CONCATENATION */
#define OP_CONCAT   ((operation_type) 0xe8) /* precedence 0xe8 */
/*------------------------------------- MISC OPERATORS */
#define is_pushiary(op) \
        (((op)>=OP_PUSHIARY_0)&&((op)<=OP_PUSHIARY_9))
#define OP_PUSHIARY_0 OP_PZ
#define OP_PZ       ((operation_type) 0xf0) /* precedence 0xf0 */
#define OP_PBZ      ((operation_type) 0xf1)
#define OP_PRZ      ((operation_type) 0xf2)
#define OP_PU       ((operation_type) 0xf3)
#define OP_PBU      ((operation_type) 0xf4)
#define OP_PRU      ((operation_type) 0xf5)
#define OP_PUSHIARY_9 OP_PRU
/**/
#define OP_NIX      ((operation_type) 0xfd) /* highest precedence */
#define OP_VAL      ((operation_type) 0xfe)
#define OP_INSVAL   ((operation_type) 0xff)
#define is_value_ary(op) ((op) >= OP_VAL)

/*===========================================================================*/
#endif
