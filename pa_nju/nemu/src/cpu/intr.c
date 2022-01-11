#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
    cpu.esp-=4;
    vaddr_write(cpu.esp,SREG_DS,4,cpu.eflags.val);
    cpu.esp-=2;
    vaddr_write(cpu.esp,SREG_DS,2,cpu.cs.val);
    cpu.esp-=4;
    vaddr_write(cpu.esp,SREG_DS,4,cpu.eip);
	uint32_t laddr=segment_translate(cpu.idtr.base+intr_no*8,SREG_DS);
	uint32_t paddr=page_translate(laddr);
	GateDesc *des=(void*)((uint32_t)hw_mem+paddr);
	uint32_t addr=(des->offset_31_16<<16)+des->offset_15_0;
	if(des->type==0xe)
	   cpu.eflags.IF=1;
	else
	   cpu.eflags.IF=0;
	cpu.eip=addr;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
