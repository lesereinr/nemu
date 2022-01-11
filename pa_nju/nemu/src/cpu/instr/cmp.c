#include "cpu/instr.h"

/*make_instr_func(cmp_i2rm_bv)
{
      OPERAND rm, imm;
    imm.data_size=8;
    rm.data_size=data_size;
    rm.data_size = data_size;      
    int len = 1;                    
    len += modrm_rm(eip + 1, &rm);
    imm.type = OPR_IMM;          
    imm.addr = eip + len;          
    imm.sreg = SREG_CS;
    operand_read(&imm);
    imm.val=sign_ext(imm.val,imm.data_size);
    rm.val=sign_ext(rm.val,rm.data_size);
    alu_sub(imm.val,rm.val,data_size);
    return len+imm.data_size/8;
}*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val=sign_ext(opr_dest.val,opr_dest.data_size);
	alu_sub(opr_src.val,opr_dest.val,data_size);
}
make_instr_impl_2op(cmp, i, rm, bv);
make_instr_impl_2op(cmp, r, rm, v);
make_instr_impl_2op(cmp, i, rm, v);
make_instr_impl_2op(cmp, r, rm, b);
make_instr_impl_2op(cmp, rm, r, v);
make_instr_impl_2op(cmp, rm, r, b);
make_instr_impl_2op(cmp, i, a, b);
make_instr_impl_2op(cmp, i, a, v);
make_instr_impl_2op(cmp, i, rm, b);