#if (defined(INT_32_BIT) || defined(INT_16_BIT))
   FAST_STATE_OFFSET_REGS();
#endif
#ifdef BUGGY
int iii;
#endif
   ssvar_value_type stateval;
   short oix;
   identifier_info_type *idinfo;
   register type_flagword_type reg_flags,reg_x_comp,reg_comp;
   register Memix ixtop;
   /*-------------------------------------------------------*/
   oix=oix_in;
   idinfo = *((*expr).operands+((Memix) oix));
   reg_flags = (*idinfo).flags;
   l_x_comp = reg_x_comp = reg_flags & X_COMP_TYPE_MASK;
   l_comp = reg_comp = reg_x_comp & COMPUTATIONAL_TYPE_MASK;
   is_ssv=bitset(reg_flags,SSVAR_TYPE);
#if !defined(QQ_RUNNING_FAST)
   if ((trace_for_const)&&(bitset(reg_flags,ANY_SPECIAL_VAR_TYPE_MASK))){
      set_arrow_sides(TRUE,FALSE);
      (*expr).in_error=TRUE;
      show_err_at_pos_on_both(E_EVALSTOVAR,this.pos,
                              (*idinfo).name,CHAR_TYPE);
      reset_arrow_sides();}
#endif
   oix++;
   if (bitset(reg_x_comp,ARRAY_TYPE)){
      if (is_ssv){
         FAST_STATE_OFFSET_COPY(&l_val.ptr.sss,&(*idinfo).ptr.sss);}
      else l_val.ptr.vvv=(*idinfo).ptr.vvv;}
   else{
      switch (is_ssv){
      case TRUE:
         stateval=state_space_value(eval_state_ssbits,(*idinfo).ptr.sss);
         switch (reg_comp)
         {
            case BOOL_TYPE:
                l_val.bbb=((stateval==0) ? FALSE : TRUE);
                break;
            default:
                l_val.iii = (int_type) stateval;
         }
         break;
      case FALSE:
         switch (reg_comp)
         {
           case INT_TYPE:
               l_val.iii = *((*idinfo).ptr.iii);
               break;
           case BOOL_TYPE:
               l_val.bbb = *((*idinfo).ptr.bbb);
               break;
           case CHAR_TYPE:
               l_val.ccc = *((*idinfo).ptr.ccc);
               break;
           default:
               l_val.rrr = *((*idinfo).ptr.rrr);
         }
         break;
      }}
   l_x_comp=(l_x_comp & SPECIAL_TYPE_MASK) | reg_comp;
   push_eval_stack(&l_val,l_x_comp,idinfo);
   /*-------------------------------------------------------*/
   return(oix);
