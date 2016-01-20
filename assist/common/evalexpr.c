/*
   This function evaluates a postfix expression beginning with index "ix0"
   in the postfix operation stack and with index "oix0" in the operand list.

   If "sub_index_expr" is true, then it is assumed that the expression is
   the index into an array or the index following a concatenation operation.
   These indicies can easily be computed given the position in the operand
   list (oix0) which is known when printing an infix expression.  Evaluation
   in "sub_index_expr" mode will cease when either a [] or ^ operation
   is encountered and only one value is on the evaluation stack.

   Theorem:   The parsing of a sub-expression for a subscript of an array
              or the second operand of a concatenation operation is complete
              if and only if a [] or ^ operation is encountered with only
              one value on the evaluation stack.

   Proof:     Both the [] and ^ operations are binary operations.

              If a [] or ^ operation is encountered with one item on the stack
              then a binary operation is encountered with only one item on the
              stack.   In order to evaluate a binary operation, two operands
              must be popped off the stack.   Since there is only one operand
              on the stack, the expression must either have been in error with
              an appropriate error message or else parsing must have begun
              with the index operand of the operation and be now complete.

              Conversely, if the parsing of a sub-expression for a subscript
              of an array or the second operand of a concatenation operation
              is complete, then the next operand on the postfix stack will
              be the [] or ^ operation.   Since sub-expression parsing began
              with the beginning of the index (second) operand of the binary
              operation and parsing of this operand is complete, there will
              be only one operand on the stack.   The next operand is therefore
              either a [] or ^ operation with only one operand on the stack.


   Theorem:   The parsing of a sub-expression for a subscript pair
              of a doubly subscripted array is complete if and only if
              a [,] operation is encountered with two values on the
              evaluation stack.

*/
   register operation_type op;
   register short ix,nix,nargs,iarg;
   short oix;
   register Memix ixtop;
#ifdef BUGGY
int iii;
char *ppp;
#endif
   /*-------------------------------------------------------*/
   empty_eval_stack();
   expr_line_number=(*expr).source_code_line_number;
#ifdef QQ_RUNNING_FAST
   nix=(*expr).n_postfix_ops;
   ix=oix=0;
#else
   nix=((ix9<0) ? (*expr).n_postfix_ops : ix9);
   oix=oix0;
   ix=ix0;
   if (trace_evaluation)
      show_infix_or_postfix((*expr).postfix_ops,(*expr).n_postfix_ops,
                            "postfix");
#endif
   op = ((ix<nix) ? (*((*expr).postfix_ops+((Memix) ix))) : OP_NIX);
#if !defined(QQ_RUNNING_FAST)
   if (ix>=nix)
      show_err_at_pos_on_both(E_INTERNAL,this.pos,
                              "Attempt to evaluate the empty expression.",
                              CHAR_TYPE);
#endif
#ifdef QQ_RUNNING_FAST
   while (ix<nix)
#else
   while ((ix<nix)&&(!sub_and_is_done(op)))
#endif
   {
#if !defined(QQ_RUNNING_FAST)
      if (trace_evaluation)
         fprintf(bugfile,"doit: ix=%d, op=%02x (%s)\n",
                          (int) ix,(int) op,operation_text+4*op);
#endif
      rou_to_perform = op_routines[op];
      if (is_value_ary(op)) oix=do_op_val(expr,oix);
      else if (is_identity_ary(op))
         /* identity ops, must be first (no rou to do) */;
      else if (is_binary_arith(op)){
         /*--------------------------------- BINARY ARITHMETIC OPERATORS */
#ifdef BUGGY
for (iii=0;iii<=eval_stack_top;++iii){
fprintf(bugfile,"   %s\n",
enk_aux_val(eval_stack_ele+iii,*(eval_stack_x_comp+iii)));}
#endif
         pop_eval_stack(&r_val,r_x_comp,r_opnd,RightsidePopFmt);
         pop_eval_stack(&l_val,l_x_comp,l_opnd,LeftsidePopFmt);
         r_comp = r_x_comp & COMPUTATIONAL_TYPE_MASK;
         l_comp = l_x_comp & COMPUTATIONAL_TYPE_MASK;
         qq_perform_eval_expr(BAD_BINARY_IX);
         brief_push_eval_stack(&l_val,l_x_comp);}
      else if (is_unary(op)){
         /*--------------------------------- UNARY OPERATORS */
#ifdef BUGGY
fprintf(bugfile,"top=%d\n",(int) eval_stack_top);
for (iii=0;iii<=eval_stack_top;++iii){
fprintf(bugfile,"   %s\n",
enk_aux_val(eval_stack_ele+iii,*(eval_stack_x_comp+iii)));}
#endif
         pop_eval_stack(&l_val,l_x_comp,l_opnd,LeftsidePopFmt);
         l_comp = l_x_comp & COMPUTATIONAL_TYPE_MASK;
         qq_perform_eval_expr(BAD_UNARY_IX);
#ifdef BUGGY
fprintf(bugfile,"push: %s\n",enk_aux_val((void *) &l_val,l_x_comp));
#endif
         brief_push_eval_stack(&l_val,l_x_comp);}
#ifdef FUTURE_PUSH_OPTIONS
      else if (is_pushiary(op)){
         qq_push_eval_stack(&push_values[op-OP_PUSHIARY_0],
                            push_comps[op-OP_PUSHIARY_0],
                            push_comps[op-OP_PUSHIARY_0]);}
#endif
      else if (is_ternary(op)){
#ifdef OBSOLETE
         if (l_opnd != NULL) is_ssv=bitset((*l_opnd).flags,SSVAR_TYPE);
#endif
         pop_eval_stack(&r_val,r_x_comp,r_opnd,RightsidePopFmt);
         pop_eval_stack(&c_val,c_x_comp,c_opnd,CenterPopFmt);
         pop_eval_stack(&l_val,l_x_comp,l_opnd,LeftsidePopFmt);
         r_comp = r_x_comp & COMPUTATIONAL_TYPE_MASK;
         c_comp = c_x_comp & COMPUTATIONAL_TYPE_MASK;
         l_comp = l_x_comp & COMPUTATIONAL_TYPE_MASK;
         qq_perform_eval_expr(BAD_TERNARY_IX);
         brief_push_eval_stack(&l_val,l_x_comp);}
      else if (is_variab_leng_ary(op)){
         /*--------------------------------- LIST BUILT-IN OPERATORS */
         pop_eval_stack(&c_val,c_x_comp,c_opnd,CenterPopFmt);
         c_comp = c_x_comp & COMPUTATIONAL_TYPE_MASK;
         nargs=c_val.iii;
         l_x_comp=INT_TYPE; l_val.iii=0;
         if (nargs>VARIAB_MAX_ARGS){
            show_err_at_pos_on_both
                (E_TOOMANYCALLING,this.pos,&c_val.iii,INT_TYPE);}
         else if (nargs<1){
            show_err_at_pos_on_both
                (E_TOOFEWCALLING,this.pos,&c_val.iii,INT_TYPE);}
         else{
            for (iarg=nargs-1;iarg>=0;--iarg){
               pop_eval_stack(m_val+iarg,*(m_x_comp+iarg),*(m_opnd+iarg),
                              MultiPopFmt);
               *(m_comp+iarg) = (*(m_x_comp+iarg)) & COMPUTATIONAL_TYPE_MASK;}
            l_x_comp=(*m_x_comp);
            qq_perform_eval_expr(BAD_VARARG_IX);}
         brief_push_eval_stack(&l_val,l_x_comp);}
      else{
         /*--------------------------------- MISC. BINARY OPERATORS */
#ifdef BUGGY
for (iii=0;iii<=eval_stack_top;++iii){
fprintf(bugfile,"   %s\n",
enk_aux_val(eval_stack_ele+iii,*(eval_stack_x_comp+iii)));}
#endif
         pop_eval_stack(&r_val,r_x_comp,r_opnd,RightsidePopFmt);
         pop_eval_stack(&l_val,l_x_comp,l_opnd,LeftsidePopFmt);
         r_comp = r_x_comp & COMPUTATIONAL_TYPE_MASK;
         l_comp = l_x_comp & COMPUTATIONAL_TYPE_MASK;
         qq_perform_eval_expr(BAD_BINARY_IX);
         brief_push_eval_stack(&l_val,l_x_comp);}
      ++ix;
      op = ((ix<nix) ? (*((*expr).postfix_ops+((Memix) ix))) : OP_NIX);}
   ixtop=(Memix) eval_stack_top;
   val_union_cpy(val,eval_stack_ele+ixtop);
   /*-------------------------------------------------------*/
   ee_ix_out=ix;
   ee_oix_out=oix;
#if !defined(QQ_RUNNING_FAST)
   if (sub_index_expr && (op==OP_IXDBY2)){
      val_union_cpy(&ee_aux_val,val);
      --ixtop;
      val_union_cpy(val,eval_stack_ele+ixtop);}
#endif
   return((type_flagword_type) 
          (*(eval_stack_x_comp+ixtop)) & COMPUTATIONAL_TYPE_MASK);

