#include "cpu/instr.h"
/*make_instr_func(push_r_v)
{
    int len=1;
    opr_src.data_size=opr_dest.data_size=data_size;
	opr_src.type = OPR_REG; 
	opr_src.addr = opcode & 0x7;
		operand_read(&opr_src);
	cpu.esp-=data_size/8;
	opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_DS;
	opr_dest.addr=cpu.esp;
	opr_dest.val=opr_src.val;
	operand_write(&opr_dest);
	return len;
}*/
static void instr_execute_1op() 
{
    operand_read(&opr_src);
 	cpu.esp-=data_size/8;
	opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_DS;
	opr_dest.addr=cpu.esp;
	opr_dest.val=opr_src.val;
	operand_write(&opr_dest);
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, v)

make_instr_func(push_i_b)
{
   int len=1;
   OPERAND imm,mem;
   imm.data_size=8;
   imm.type=OPR_IMM;
   imm.addr=eip+1;
   imm.sreg=SREG_CS;
   len+=imm.data_size/8;
   print_asm_1("push", "b", len, &opr_src);
   cpu.esp-=data_size/8;
   operand_read(&imm);
   sign_ext(imm.val,imm.data_size);
   mem.addr=cpu.esp;
   mem.type=OPR_MEM;
   mem.data_size=data_size;
   mem.sreg=SREG_DS;
   mem.val=imm.val;
   operand_write(&mem);
   return len;
}
make_instr_func(pusha)
{
    OPERAND temp;
    temp.data_size=32;
    temp.type=OPR_MEM;
    temp.sreg=SREG_DS;
    //eax
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.eax;
    operand_write(&temp);
    //ecx
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.ecx;
    operand_write(&temp);
    //edx
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.edx;
    operand_write(&temp);
    //ebx
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.ebx;
    operand_write(&temp);
    //xxx(esp)
    cpu.esp-=4;
    //temp.addr=cpu.esp;
    //temp.val=cpu.eax;
    //operand_write(&temp);
    //ebp
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.ebp;
    operand_write(&temp);
    //esi
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.esi;
    operand_write(&temp);
    //edi
    cpu.esp-=4;
    temp.addr=cpu.esp;
    temp.val=cpu.edi;
    operand_write(&temp);
    print_asm_0("pusha","",1);
    return 1;
}