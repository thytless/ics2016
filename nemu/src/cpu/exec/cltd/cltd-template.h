#include<cpu/exec/template-start.h>

#define instr cltd

make_helper(cltd){
	if(ops_decoded.is_operand_size_16){
		if(REG(R_AX) >> 0xf)
			cpu.edx |= 0x0000FFFF;
		else
			cpu.edx &= 0xFFFF0000; 
		print_asm("cwtl");	
	}
	else{
		if(REG(R_EAX) >> 0x1f)
			cpu.edx = 0xFFFFFFFF;
		else
			cpu.edx = 0; 
		print_asm("cltd");
	}
	return 1;
}
#include<cpu/exec/template-end.h>


