#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt)
{
    int len=1;
    OPERAND temp;
    len+=modrm_rm(eip+1,&temp);
    print_asm_1("lgdt","",len,&temp);
    temp.data_size=16;
    operand_read(&temp);
    cpu.gdtr.limit=temp.val;
    temp.data_size=32;
    temp.addr+=2;
    operand_read(&temp);
    cpu.gdtr.base=temp.val;
    return len;
}