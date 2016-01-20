#if !defined(_ZZ_q_objdefs_DEFINED)
#define _ZZ_q_objdefs_DEFINED
/*===========================================================================*/

                     /*************************/
                     /*************************/
                     /****                 ****/
                     /****   OBJECT CODE   ****/
                     /****   TABLE TYPES   ****/
                     /****                 ****/
                     /*************************/
                     /*************************/

#define OBJ_HEADER        ((object_table_type) 0xf0)
#define OBJ_IDTABLE       ((object_table_type) 0xf1)
#ifdef OBSOLETE
#define OBJ_NUMTABLE      ((object_table_type) 0xf2)
#endif
#define OBJ_OPREC         ((object_table_type) 0xf3)
#define OBJ_VERBATIM_HEAD ((object_table_type) 0xf4)
#define OBJ_VERBATIM_TAIL ((object_table_type) 0xf5)
#define OBJ_EOF           ((object_table_type) 0xff)

#define OBJ_CHAR_DATA      ((object_table_type) 0x01)/* must be ordered */
#define OBJ_BOOL_DATA      ((object_table_type) 0x02)/* in order to */
#define OBJ_INT_DATA       ((object_table_type) 0x03)/* correspond precisely */
#define OBJ_REAL_DATA      ((object_table_type) 0x04)/* to types in "defs.h" */
#define OBJ_SOFF_DATA      ((object_table_type) 0x05)

#define OBJ_CHAR_VARDATA   ((object_table_type) 0x09)/* must be ordered */
#define OBJ_BOOL_VARDATA   ((object_table_type) 0x0a)/* in order to */
#define OBJ_INT_VARDATA    ((object_table_type) 0x0b)/* correspond precisely */
#define OBJ_REAL_VARDATA   ((object_table_type) 0x0c)/* to types in "defs.h" */
#define OBJ_SOFF_VARDATA   ((object_table_type) 0x0d)

#define OBJ_EXPR     ((object_table_type) 0x10)/* COMPUTATIONAL_TYPE_COUNT */
#define OBJ_OPERANDS ((object_table_type) 0x11)
#define OBJ_OPS      ((object_table_type) 0x12)
#define OBJ_DST_0    ((object_table_type) 0x13)/* application data structs */
#define OBJ_DST_9    ((object_table_type) 0x2f)/* application data structs */

#define MAX_OBJ_COUNTER_DIM ((object_table_type) 0x30) /* one more than max */

#ifdef MUST_ALIGN
#define PAD_REQUIRED(amt,boundary) \
        if (amt>0) { amt = 1 + (amt-1)/boundary; amt *= boundary; }
#endif


/*
   See also: objvars.h
*/
/*===========================================================================*/
#endif
