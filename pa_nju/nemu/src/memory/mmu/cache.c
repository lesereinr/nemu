#include "memory/mmu/cache.h"
#include"memory/memory.h"
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
// init the cache
void init_cache()
{
   /* int i;
    for( i=0;i<1024;++i)
    {
    Cache[i].vaild=0;
    }*/
	// implement me in PA 3-1
	
}
// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)//,CacheLine* cache)
{
    /*
    uint32_t sign,group,addr;
    sign=(paddr>>13)&0x7FFFF;
    group=(paddr>>6)&0x7F;
    addr=paddr&0x3F;
    memcpy(hw_mem+paddr,&data,len);
    int i;
    for(i=0;i<8;++i)
    {
        if(cache[group*8+i].vaild==1&&cache[group*8+i].sign==sign)
        {
            if(addr+len>64)
            {
                cache_write(paddr,64-addr,data,Cache);
                cache_write(paddr+64-addr,len-64+addr,data>>(8*(64-addr)),Cache);
            }
            else
            memcpy(cache[group*8+i].block+addr,&data,len);
            break;
        }
    }*/
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)//,CacheLine* cache)
{
    /*uint32_t ret,sign,group,addr;
    sign=(paddr>>13)&0x7FFFF;
    group=(paddr>>6)&0x7F;
    addr=paddr&0x3F;
	int i;
	for(i=0;i<8;++i)
	{
	    if(cache[group*8+i].vaild==1&&cache[group*8+i].sign==sign)
	    {
	        if(addr+len>64)
	        {
	            uint32_t left=0,right=0;
	            memcpy(&left,cache[group*8+i].block+addr,64-addr);
	            right=cache_read(paddr+64-addr,len-64+addr,Cache);
	            right=right<<(8*(64-addr));
	            ret=left+right;
	        }
	        else
               memcpy(&ret,cache[group*8+i].block+addr,len);
	        return ret;
	    }
	}
	    memcpy(&ret,hw_mem+paddr,len);
	    for(i=0;i<8;++i)
	    {
	        if(cache[group*8+i].vaild==0)
	        {
	            cache[group*8+i].vaild=1;
	            cache[group*8+i].sign=sign;
	            memcpy(cache[group*8+i].block,hw_mem+paddr-addr,64);
	            return ret;
	        }
	    }
	    srand(time(0));
	    i=rand()%8;
	    cache[group*8+i].vaild=1;
	    cache[group*8+i].sign=sign;
	    memcpy(cache[group*8+i].block,hw_mem+paddr-addr,64);
	    return ret;*/
	    return 0;
}

