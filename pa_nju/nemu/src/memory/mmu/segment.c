#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	 uint32_t base=cpu.segReg[sreg].base;
	 return base+offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	 
	 uint32_t addr,base,limit;
	 addr=(uint32_t)hw_mem+cpu.gdtr.base+(cpu.segReg[sreg].index<<3);
	 SegDesc* seg=(void*)addr;
	 limit=(seg->limit_19_16<<16)+seg->limit_15_0;
	 base=(seg->base_31_24<<24)+(seg->base_23_16<<16)+seg->base_15_0;
	 assert(seg->granularity==1);
	 cpu.segReg[sreg].limit=limit;
	 cpu.segReg[sreg].base=base;
	 
}
