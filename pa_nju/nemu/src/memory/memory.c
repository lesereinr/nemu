#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include"memory/mmu/cache.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];
//CacheLine Cache[1024];
uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
    //ret=cache_read(paddr,len,Cache);
    ret=cache_read(paddr,len);
#else
    int num=is_mmio(paddr);
    if(num==-1)
	ret = hw_mem_read(paddr, len);
	else
	ret=mmio_read(paddr,len,num);
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data)
{
#ifdef CACHE_ENABLED
    //cache_write(paddr,len,data,Cache);
    cache_write(paddr,len,data);
#else
    int num=is_mmio(paddr);
    if(num==-1)
	hw_mem_write(paddr, len, data);
	else
	mmio_write(paddr,len,data,num);
#endif
}

uint32_t laddr_read(laddr_t laddr, size_t len)
{
      assert(len == 1 || len == 2 || len == 4);
      paddr_t addr,addrl,addrr;
    if(cpu.cr0.pe==1&&cpu.cr0.pg==1)
    {
        if(((laddr&0xFFF)+len)>0x1000)
        {
            uint32_t lenl,lenr,left,right;
            lenl=0x1000-(laddr&0xFFF);
            addrl=page_translate(laddr);
            left=paddr_read(addrl,lenl);
            lenr=(laddr&0xFFF)+len-0x1000;
            addrr=page_translate(laddr+0x1000-(laddr&0xFFF));
            right=paddr_read(addrr,lenr)<<(8*lenl);
            return left+right;
        }
        else
        {
            addr=page_translate(laddr);
            return paddr_read(addr,len);
        }
    }
    else
    return paddr_read(laddr,len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
    assert(len == 1 || len == 2 || len == 4);
     paddr_t addr,addrl,addrr;
    if(cpu.cr0.pe==1&&cpu.cr0.pg==1)
    {
        if(((laddr&0xFFF)+len)>0x1000)
        {
            uint32_t lenl,lenr;
            lenl=0x1000-(laddr&0xFFF);
            addrl=page_translate(laddr);
            paddr_write(addrl,lenl,data & ((1 << (lenl * 8 + 1)) - 1));
            lenr=(laddr&0xFFF)+len-0x1000;
            addrr=page_translate(laddr+0x1000-(laddr&0xFFF));
            paddr_write(addrr,lenr,data>>(8*lenl));
            return;
        }
        else
        {
            addr=page_translate(laddr);
            paddr_write(addr,len,data);
            return;
        }
    }
    else
    paddr_write(laddr,len,data);
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
    vaddr_t addr;
    if(cpu.cr0.pe==1)
       addr=segment_translate(vaddr,sreg);
    else
       addr=vaddr;
    return laddr_read(addr,len);
#endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	laddr_write(vaddr, len, data);
#else
    vaddr_t addr;
    if(cpu.cr0.pe==1)
       addr=segment_translate(vaddr,sreg);
    else
       addr=vaddr;
    laddr_write(addr,len,data);
#endif
}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
    init_cache();
#endif
#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}
