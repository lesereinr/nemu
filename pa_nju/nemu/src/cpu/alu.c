#include "cpu/cpu.h"
#include "cpu/alu.h"
#include "cpu/reg.h"
//add标志位的实现
//begin
void set_CF_add(uint32_t result, uint32_t src, size_t data_size)
{
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=result<src;
}
void set_ZF(uint32_t result,  size_t data_size)
{
    result=result&(0xFFFFFFFF>>(32-data_size));
    cpu.eflags.ZF=(result==0);
}
void set_SF(uint32_t result,  size_t data_size)
{
    result=sign_ext(result& 0xFFFFFFFF>>(32-data_size),data_size);
    cpu.eflags.SF=sign(result);
}
void set_PF(uint32_t result)
{
    int i=0;
    if((result&0x00000001)==0x00000001)  i++;
    if((result&0x00000002)==0x00000002)  i++;
    if((result&0x00000004)==0x00000004)  i++;
    if((result&0x00000008)==0x00000008)  i++;
    if((result&0x00000010)==0x00000010)  i++;
    if((result&0x00000020)==0x00000020)  i++;
    if((result&0x00000040)==0x00000040)  i++;
    if((result&0x00000080)==0x00000080)  i++;
    if(i%2==1)
    cpu.eflags.PF=0;
    else
    cpu.eflags.PF=1;
}
void set_OF_add(uint32_t result,uint32_t src, uint32_t dest, size_t data_size)
{
    switch(data_size)
    {
        case 8:
               result=sign_ext(result & 0xFF,8);
               src=sign_ext(src & 0xFF,8);
               dest=sign_ext(dest & 0xFF,8);
               break;
        case 16:
               result=sign_ext(result & 0xFFFF,16);
               src=sign_ext(src & 0xFFFF,16);
               dest=sign_ext(dest & 0xFFFF,16);
               break;
        default: break;
    }
    if(sign(src)==sign(dest))
    {
        if(sign(dest)!=sign(result))
          cpu.eflags.OF=1;
        else
          cpu.eflags.OF=0;
    }
    else
    cpu.eflags.OF=0;
}
//end
//adc标志位的实现
//begin
void set_CF_adc(uint32_t result, uint32_t src, size_t data_size)
{
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    if(cpu.eflags.CF==1)
    cpu.eflags.CF=(result<=src);
    else
    cpu.eflags.CF=(result<src);
}
//end
//sub标志位实现
//begin
void set_CF_sub(uint32_t dest, uint32_t src, size_t data_size)
{
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=dest<src;
}
void set_OF_sub(uint32_t result,uint32_t src, uint32_t dest, size_t data_size)
{
    switch(data_size)
    {
        case 8:
               result=sign_ext(result & 0xFF,8);
               src=sign_ext(src & 0xFF,8);
               dest=sign_ext(dest & 0xFF,8);
               break;
        case 16:
               result=sign_ext(result & 0xFFFF,16);
               src=sign_ext(src & 0xFFFF,16);
               dest=sign_ext(dest & 0xFFFF,16);
               break;
        default: break;
    }
    if(sign(src)!=sign(dest))
    {
        if(sign(dest)!=sign(result))
          cpu.eflags.OF=1;
        else
          cpu.eflags.OF=0;
    }
    else
    cpu.eflags.OF=0;
}
//end
//add标志位的实现
//begin
void set_CF_sbb(uint32_t dest, uint32_t src, size_t data_size)
{
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    if(cpu.eflags.CF==1)
    cpu.eflags.CF=dest<=src;
    else
    cpu.eflags.CF=dest<src;
}
//end
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
    uint32_t res=0;
	res=dest+src;
	set_CF_add(res,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_add(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res=0;
	res=dest+src+cpu.eflags.CF;
	set_CF_adc(res,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_add(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res=0;
	res=dest-src;
	set_CF_sub(dest,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_sub(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
    uint32_t res=0;
	res=dest-src-cpu.eflags.CF;
	set_CF_sbb(dest,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_sub(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
    uint64_t res=0;
    if(data_size==32)
    res=(uint64_t)src*(uint64_t)dest;
    else
    res=src*dest;
    if(data_size==8)
    cpu.eflags.OF=cpu.eflags.CF=((res&0xFF00)!=0);
    else if(data_size==16)
    cpu.eflags.OF=cpu.eflags.CF=((res&0xFFFF0000)!=0);
    else
    cpu.eflags.OF=cpu.eflags.CF=((res&0xFFFFFFFF00000000)!=0);
    return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	 uint64_t res=0;
    if(data_size==32)
    res=(uint64_t)src*(uint64_t)dest;
    else
    res=src*dest;
    return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	uint32_t res=0;
	if(src==0)
	return 0;
	else
	res=dest/src;
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	uint32_t res=0;
	if(src==0)
	return 0;
	else
	res=dest/src;
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
    uint32_t res=0;
	if(src==0)
	return 0;
	else
	res=dest%src;
	return res;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	uint32_t res=0;
	if(src==0)
	return 0;
	else
	res=dest%src;
	return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res=0;
	res=src&dest;
	cpu.eflags.OF=cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
    uint32_t res=0;
	res=src^dest;
	cpu.eflags.OF=cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res=0;
	res=src|dest;
	cpu.eflags.OF=cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	cpu.eflags.CF=0;
	dest=dest&(0xFFFFFFFF>>(32-data_size));
	while(src)
	{
		if(data_size==8)
			cpu.eflags.CF=((dest&0x80)==0x80);
		else if(data_size==16)
			cpu.eflags.CF=((dest&0x8000)==0x8000);
 		else
			cpu.eflags.CF=((dest&0x80000000)==0x80000000);
		dest=dest<<1;
		src--;
	}
	set_PF(dest);
	set_ZF(dest,data_size);
	set_SF(dest,data_size);
	return dest&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	cpu.eflags.CF=0;
	dest=dest&(0xFFFFFFFF>>(32-data_size));
		while(src)
	{
		if(data_size==8)
			cpu.eflags.CF=((dest&0x01)==0x01);
		else if(data_size==16)
			cpu.eflags.CF=((dest&0x0001)==0x0001);
 		else
			cpu.eflags.CF=((dest&0x00000001)==0x00000001);
		dest=dest>>1;
		src--;
	}
	set_PF(dest);
	set_ZF(dest,data_size);
	set_SF(dest,data_size);
	return dest&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
    dest=dest&(0xFFFFFFFF>>(32-data_size));
	if(data_size==8&&(dest&0x00000080)==0x00000080)
		while(src){
			cpu.eflags.CF=((dest&0x01)==0x01);
			dest=dest>>1;
			dest=dest^0x00000080;
			src--;
		}
	else if(data_size==16&&(dest&0x00008000)==0x00008000)
		while(src){
			cpu.eflags.CF=((dest&0x0001)==0x0001);
			dest=dest>>1;
			dest=dest^0x00008000;
			src--;
		}
	else if(data_size==32&&(dest&0x80000000)==0x80000000)
		while(src){
			cpu.eflags.CF=((dest&0x00000001)==0x00000001);
			dest=dest>>1;
			dest=dest^0x80000000;
			src--;
		}
	else
		return alu_shr(src,dest,data_size);
	set_PF(dest);
	set_ZF(dest,data_size);
	set_SF(dest,data_size);
	return dest&(0xFFFFFFFF>>(32-data_size));	
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	return alu_shl(src,dest,data_size);
#endif
}
