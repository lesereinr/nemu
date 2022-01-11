#include "cpu/instr.h"
make_instr_func(lidt)
{
    int len=1;
    OPERAND temp;
    len+=modrm_rm(eip+1,&temp);
    print_asm_1("lidt","",len,&temp);
    temp.data_size=16;
    operand_read(&temp);
    cpu.idtr.limit=temp.val;
    temp.data_size=32;
    temp.addr+=2;
    operand_read(&temp);
    cpu.idtr.base=temp.val;
    return len;
}
