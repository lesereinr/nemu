#include "cpu/instr.h"
make_instr_func(call_near)
{
    OPERAND imm,mem;
    imm.data_size=data_size;
    imm.addr=cpu.eip+1;
    imm.type=OPR_IMM;
    imm.sreg=SREG_CS;
    operand_read(&imm);
    cpu.esp-=data_size/8;
    mem.data_size=data_size;
    mem.addr=cpu.esp;
    mem.type=OPR_MEM;
    mem.sreg=SREG_DS;
    mem.val=cpu.eip+1+data_size/8;
    operand_write(&mem);
    int length=sign_ext(imm.val,data_size);
    cpu.eip+=length+1+data_size/8;
    return 0;
}
make_instr_func(call_near_indirect)
{
    OPERAND imm,mem;
    int len=1;
    imm.data_size=data_size;
   len+=modrm_rm(eip+1,&imm);
    operand_read(&imm);
    cpu.esp-=data_size/8;
    mem.data_size=data_size;
    mem.addr=cpu.esp;
    mem.type=OPR_MEM;
    mem.sreg=SREG_DS;
    mem.val=cpu.eip+len;
    operand_write(&mem);
   if(data_size==16)
   cpu.eip=imm.val&0xffff;
   else
   cpu.eip=imm.val;
    return 0;
}