#if !defined(_ZZ_q_astdefs_DEFINED)
#define _ZZ_q_astdefs_DEFINED
/*===========================================================================*/
#define MAXNCH_ENCODED_STATE 4096
#define MAXNCH_MODEL_TRANSITION_LINE 4160 /* 4096+64 */
/*
   For application data structures.   See also: "objdefs.h"
*/
#define OBJ_VARINF   ((object_table_type) 0x13)
#define OBJ_SETRNGE  ((object_table_type) 0x14)
#define OBJ_PIX      ((object_table_type) 0x15)
#define OBJ_BOOLTEST ((object_table_type) 0x16)
#define OBJ_TRANTO   ((object_table_type) 0x17)
#define OBJ_IF       ((object_table_type) 0x18)
#define OBJ_FOR      ((object_table_type) 0x19)
#define OBJ_CALC     ((object_table_type) 0x1a)
#define OBJ_CODE_0   ((object_table_type) 0x28)
/**/
#define OBJ_CODE_PREAMBLE OBJ_CODE_0
#define OBJ_CODE_ASSERT  (OBJ_CODE_0+1+OPCODE_ASSERT)
#define OBJ_CODE_DEATHIF (OBJ_CODE_0+1+OPCODE_DEATHIF)
#define OBJ_CODE_PRUNEIF (OBJ_CODE_0+1+OPCODE_PRUNEIF)
#define OBJ_CODE_TRANTO  (OBJ_CODE_0+1+OPCODE_TRANTO)
#define OBJ_CODE_CALC    (OBJ_CODE_0+1+OPCODE_CALC)
#define OBJ_CODE_CALC_T  (OBJ_CODE_0+1+OPCODE_CALC_T)
/**/
#define EMPTY_MODE                      ((mode_flag_type) 0x00)
#define SAME_MODE                       EMPTY_MODE
/**/
#define MODE_SECT_MASK                  ((mode_flag_type) 0x0f)
#define SETUP_SECTION_MODE              ((mode_flag_type) 0x01)
#define START_SECTION_MODE              ((mode_flag_type) 0x02)
#define RULE_SECTION_MODE               ((mode_flag_type) 0x04)
/**/
#define ANY_PRE_RULE_SECTION_MODE       ((mode_flag_type) 0x03)
#define ANY_SECTION_MODE                ((mode_flag_type) 0x0f)
#define ANY_POST_START_SECTION_MODE     ((mode_flag_type) 0x36)
/**/
#define GOTO_SETUP_SECTION_MODE         ((mode_flag_type) 0x11)
#define GOTO_START_SECTION_MODE         ((mode_flag_type) 0x12)
#define GOTO_RULE_SECTION_MODE          ((mode_flag_type) 0x14)
/*===========================================================================*/
#define OPCODE_ASSERT    ((opcode_type) 0)
#define OPCODE_DEATHIF   ((opcode_type) 1)
#define OPCODE_PRUNEIF   ((opcode_type) 2)
#define OPCODE_TRANTO    ((opcode_type) 3)
#define OPCODE_CALC      ((opcode_type) 4) /* booltest sections */
#define OPCODE_CALC_T    ((opcode_type) 5) /* tranto sections */
/**/
#define RULE_OPCODE_INDEX_COUNT 6
/**/
#define OPCODE_START     ((opcode_type) 6)
#define OPCODE_SPACE     ((opcode_type) 7)
/**/
#define OPCODE_BLOCK_IF  ((opcode_type) 8)
#define OPCODE_FOR_LOOP  ((opcode_type) 9)
/**/
#define OPCODE_BEGIN     ((opcode_type) 10)
#define OPCODE_END       ((opcode_type) 11)
#define OPCODE_GOTO      ((opcode_type) 12)
#define OPCODE_GOSUB     ((opcode_type) 13)
#define OPCODE_RETURN    ((opcode_type) 14)
/**/
#define OPCODE_COUNT 15
#define OPCODE_REG_MASK ((opcode_type) 0x0f)
#define OPCODE_SEC_MASK ((opcode_type) 0xf0)
#define OPCODE_SEC_SHIFT ((unsigned) 4)
/*===========================================================================*/
#define RATE_EXPRESSION_COUNT_MASK ((short) 0x00ff)
#define BY_FAST_RATE               ((short) 0x0100)
#define BY_RATE_WITH_ANGLE         ((short) 0x0200)
#define BY_FAST_TRANSITION_MASK    ((short) 0x0300)
/*===========================================================================*/
#endif
