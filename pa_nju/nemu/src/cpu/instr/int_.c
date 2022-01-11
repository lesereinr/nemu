#include "cpu/intr.h"
#include "cpu/instr.h"
make_instr_func(int_)
{
    OPERAND imm;
    imm.data_size=8;
    imm.type=OPR_IMM;
    imm.sreg=SREG_CS;
    imm.addr=eip+1;
    operand_read(&imm);
    raise_sw_intr(imm.val);
    print_asm_1("int_","",2,&imm);
    return 0;
}
