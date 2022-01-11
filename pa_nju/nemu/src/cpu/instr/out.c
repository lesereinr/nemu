#include "cpu/instr.h"
#include "device/port_io.h"
make_instr_func(out_b)
{
    pio_write(cpu.edx,1,cpu.eax);
    print_asm_0("out","b",1);
    return 1;
}
make_instr_func(out_v)
{
    pio_write(cpu.edx,data_size/8,cpu.eax);
    print_asm_0("out","v",1);
    return 1;
}
