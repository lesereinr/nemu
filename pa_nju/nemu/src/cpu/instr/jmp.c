#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}
make_instr_func(jmp_short)
{
    OPERAND imm;
    imm.type=OPR_IMM;
    imm.addr=eip+1;
    imm.sreg=SREG_CS;
    imm.data_size=8;
    operand_read(&imm);
    cpu.eip+=sign_ext(imm.val,imm.data_size);
    return 2;
}
make_instr_func(jmp_near_indirect)
{
    int len=1;
    OPERAND temp;
    temp.data_size=data_size;
    len+=modrm_rm(eip+1,&temp);
    operand_read(&temp);
    print_asm_1("jmp","",len,&temp);
    if(temp.data_size==16)
    cpu.eip=temp.val&0xffff;
    else
    cpu.eip=temp.val;
    return 0;
}
make_instr_func(jmp_far_imm)
{
    OPERAND temp;
    temp.type=OPR_IMM;
    temp.sreg=SREG_CS;
    temp.data_size=32;
    temp.addr=eip+1;
    operand_read(&temp);
    if(data_size==16)
        cpu.eip=temp.val&0xFFFF;
    else
        cpu.eip=temp.val;
    cpu.cs.val=instr_fetch(eip+5,2);
    load_sreg(1);
    print_asm_1("jmp","",7,&temp);
    return 0;
}