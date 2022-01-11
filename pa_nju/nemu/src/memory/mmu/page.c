#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
    
    PDE pde;
    PTE pte;
    uint32_t dir = laddr >> 22;
    uint32_t page = (laddr >> 12) & 0x3FF;
    uint32_t offset = laddr & 0xFFF;
    pde.val = paddr_read((cpu.cr3.pdbr << 12) + (dir << 2), 4);
    assert(pde.present == 1);
    pte.val = paddr_read((pde.page_frame << 12) + (page << 2), 4);
    assert(pte.present == 1);
    return (pte.page_frame << 12) + offset;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
