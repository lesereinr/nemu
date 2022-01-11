#include "cpu/instr.h"
#include "device/port_io.h"
make_instr_func(in_b)
{
    cpu.eax=pio_read(cpu.edx,1);
    print_asm_0("in","b",1);
    return 1;
}
make_instr_func(in_v)
{
    cpu.eax=pio_read(cpu.edx,data_size/8);
    print_asm_0("in","v",1);
    return 1;
}
