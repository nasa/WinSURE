/*
===============================================================================

        System Dependent functions required by ASSIST and SURE compiler
        programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)

===============================================================================
*/

#include "common.h"

#include "objdefs.h"
#include "objtypes.h"
#include "objvars.h"

#include "astdefs.h"
#include "asttypes.h"
#include "astvars.h"

#include "ast_lexio_vars.h"

#include "fix_ext.h"
#include "obj_ext.h"

/*===========================================================================*/
void set_map_addr0(char *ppp)
{
   /*-------------------------------------------------------*/
   map_ppp0=ppp;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fixup_an_expr(expression_type *expr)
{
   /*-------------------------------------------------------*/
#ifdef BUGGY
/*?????*/ fprintf(stderr," %08x: p:%03x i:%03x o:%03x",(int) expr,(int) (*expr).postfix_ops,(int) (*expr).infix_ops,(int) (*expr).operands);
#endif
   (*expr).postfix_ops = (operation_type *) FIXUP((*expr).postfix_ops,OBJ_OPS);
   (*expr).infix_ops = (operation_type *) FIXUP((*expr).infix_ops,OBJ_OPS);
   (*expr).operands = (operand_type *) FIXUP((*expr).operands,OBJ_OPERANDS);
#ifdef BUGGY
/*?????*/ fprintf(stderr," p:%08x i:%08x o:%08x\n",(int) expr,(int) (*expr).postfix_ops,(int) (*expr).infix_ops,(int) (*expr).operands);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void *fixup_pointer(relative_address_type reladdr,short ix)
{
   char *ans;
   /*-------------------------------------------------------*/
   if (reladdr==NIX_ADDR) ans = ((char *) NULL);
   else if (ix>=0) ans = section_start_addrs[ix] + reladdr;
   else ans = map_ppp0 + reladdr;
   /*-------------------------------------------------------*/
   return((void *) ans);
}
/*===========================================================================*/
void fixup_identifier_table(long count)
{
   identifier_info_type *idinfo,*idovf;
   object_table_type ooo,ooostore;
#ifdef BUGGY
long lans;
#endif
   /*-------------------------------------------------------*/
   nele_identifier_table=(Memix) count;
   idinfo=identifier_table;
   idovf=idinfo+nele_identifier_table;
   for (;idinfo<idovf;++idinfo){
      ooo = cvt_to_object_table_type((*idinfo).flags);
      Set_ooostore();
#ifdef BUGGY
fprintf(bugfile,"idtab[%d] reladdr=%08lx\n",
(int) (idinfo-identifier_table),
(long) (*idinfo).ptr.relative_address);
lans=(long) FIXUP((*idinfo).ptr.relative_address,ooostore);
fprintf(bugfile,"lans=%08lx\n",lans);
#endif
      (*idinfo).ptr.vvv=FIXUP((*idinfo).ptr.relative_address,ooostore);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fixup_expressions()
{
   Memix iii,nnn;
   expression_type *xprs,*xprsovf;
   operand_type opnd,*opnd_arr;
   /*-------------------------------------------------------*/
   for_all_ptrs(OBJ_EXPR,xprs,xprsovf,expression_type)
       fixup_an_expr(xprs);
   opnd_arr=(operand_type *) section_start_addrs[OBJ_OPERANDS];
   nnn = memory_bytes_required[OBJ_OPERANDS]/sizeof(operand_type);
   for (iii=0;iii<nnn;++iii){
      opnd = *(opnd_arr+iii);
      *(opnd_arr+iii) = absolute_identifier((relative_address_type) opnd);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fixup_user()
{
   set_range_type *sets,*setsovf;
   state_space_picture_type *pixs,*pixsovf;
   booltest_type *tests,*testsovf;
   tranto_clause_type *tnts,*tntsovf;
   calc_assign_type *clcs,*clcsovf;
   block_if_type *bifs,*bifsovf;
   for_loop_type *forngs,*forngsovf;
   instruction_type *insts,*instsovf;
   vars_union_type *vrus,*vrusovf;
   object_table_type ooo,oooend,ooofixup,ooobgnknt,ooobgn0;
   relative_address_type reladdr;
   Boolean begin,start;
   /*-------------------------------------------------------*/
   deathif_count = pruneif_count = assert_count = 0;
   ooobgn0=ooobgnknt=OBJ_CODE_0+1;
   oooend=OBJ_CODE_0+((object_table_type) RULE_OPCODE_INDEX_COUNT);
   for (ooo=OBJ_CODE_0;ooo<=oooend;++ooo){
      for_all_ptrs(ooo,insts,instsovf,instruction_type){
         begin=start=FALSE;
         ooofixup=OBJ_CODE_0;
         if ((*insts).opcode==OPCODE_BLOCK_IF) ooofixup=OBJ_IF;
         else if ((*insts).opcode==OPCODE_FOR_LOOP) ooofixup=OBJ_FOR;
         else if ((*insts).opcode==OPCODE_ASSERT){
            ooofixup=OBJ_BOOLTEST; ++assert_count;}
         else if ((*insts).opcode==OPCODE_DEATHIF){
            ooofixup=OBJ_BOOLTEST; ++deathif_count;}
         else if ((*insts).opcode==OPCODE_PRUNEIF){
            ooofixup=OBJ_BOOLTEST; ++pruneif_count;}
         else if ((*insts).opcode==OPCODE_TRANTO) ooofixup=OBJ_TRANTO;
         else if ((*insts).opcode==OPCODE_CALC) ooofixup=OBJ_CALC;
         else if ((*insts).opcode==OPCODE_START){
            ooofixup=OBJ_TRANTO;
            start=TRUE;}
         else if ((*insts).opcode==OPCODE_BEGIN){
            ooofixup=ooobgnknt++;
            begin=TRUE;}
         else if ((*insts).opcode==OPCODE_END) ooofixup=ooo;
         else if ((*insts).opcode==OPCODE_SPACE) ooofixup=OBJ_PIX;
         else if ((*insts).opcode==OPCODE_RETURN) ooofixup=ooo;
         else if ((*insts).opcode==OPCODE_GOTO) ooofixup=ooo;
         else if ((*insts).opcode==OPCODE_GOSUB) ooofixup=ooo;
#ifdef BUGGY
else{
#ifdef INTERNAL_BUGGY
fprintf(stderr,"bad opcode=%d\n",(int) ((*insts).opcode));
#endif
ooofixup=OBJ_CODE_0;
error_this_pos(E_INTERNAL);}
#endif
         (*insts).ptr.vvv=FIXUP((*insts).ptr.reladdr,ooofixup);
         if (begin) initial_start_instr[ooofixup-ooobgn0]=(*insts).ptr.vvv;
         if (start) start_tranto_ptr=(tranto_clause_type *) (*insts).ptr.vvv;
         if (ooofixup==OBJ_PIX){
            state_space_picture_ptr =
               (state_space_picture_type *) (*insts).ptr.vvv;}
         else if (ooofixup==OBJ_IF){
            bifs = (block_if_type *) (*insts).ptr.vvv;
            INST_FIXUP((*bifs).then_clause,ooo);
            INST_FIXUP((*bifs).else_clause,ooo);}
         else if (ooofixup==OBJ_FOR){
            forngs = (for_loop_type *) (*insts).ptr.vvv;
            INST_FIXUP((*forngs).body,ooo);}}}
   for_all_ptrs(OBJ_IF,bifs,bifsovf,block_if_type){
      (*bifs).then_test=(expression_type *) FIXUP((*bifs).then_test,OBJ_EXPR);}
   for_all_ptrs(OBJ_FOR,forngs,forngsovf,for_loop_type){
      (*forngs).ident = 
           absolute_identifier((relative_address_type) (*forngs).ident);
      (*forngs).set_ranges = (set_range_type *)
           FIXUP((*forngs).set_ranges,OBJ_SETRNGE);}
   for_all_ptrs(OBJ_BOOLTEST,tests,testsovf,booltest_type)
      (*tests).expr=(expression_type *) FIXUP((*tests).expr,OBJ_EXPR);
   for_all_ptrs(OBJ_TRANTO,tnts,tntsovf,tranto_clause_type){
      (*tnts).sex.exprs=(expression_type *) FIXUP((*tnts).sex.exprs,OBJ_EXPR);
      (*tnts).sex.vars = (operand_type *)
           FIXUP((*tnts).sex.vars,OBJ_OPERANDS);
      (*tnts).rate_exprs=(expression_type *)
           FIXUP((*tnts).rate_exprs,OBJ_EXPR);}
   for_all_ptrs(OBJ_CALC,clcs,clcsovf,calc_assign_type){
      (*clcs).idinfo =
           absolute_identifier((relative_address_type) (*clcs).idinfo);
      (*clcs).expr=(expression_type *) FIXUP((*clcs).expr,OBJ_EXPR);}
   for_all_ptrs(OBJ_PIX,pixs,pixsovf,state_space_picture_type){
      (*pixs).varu = (vars_union_type *) FIXUP((*pixs).varu,OBJ_VARINF);
      (*pixs).is_nested=(Boolean *) FIXUP((*pixs).is_nested,OBJ_BOOL_DATA);}
   for_all_ptrs(OBJ_SETRNGE,sets,setsovf,set_range_type){
      (*sets).lower_bound=(expression_type *)
           FIXUP((*sets).lower_bound,OBJ_EXPR);
      (*sets).upper_bound=(expression_type *)
           FIXUP((*sets).upper_bound,OBJ_EXPR);}
   for_all_ptrs(OBJ_VARINF,vrus,vrusovf,vars_union_type){
      if (bitset((*vrus).relative_address,RELADDR_FLAG)){
         reladdr = RELADDR_FLAG;
         reladdr = (~reladdr);
         reladdr = (*vrus).relative_address & reladdr;
         (*vrus).id_info = absolute_identifier(reladdr);}
      else{
         (*vrus).nested_space_picture = (state_space_picture_type *)
             FIXUP((*vrus).relative_address,OBJ_PIX);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
