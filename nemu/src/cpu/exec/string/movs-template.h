#include<cpu/exec/template-start.h>

#define instr movs
make_helper(concat(movs_,SUFFIX)){
	uint32_t read = swaddr_read(cpu.esi,DATA_BYTE);
	swaddr_write(cpu.edi,DATA_BYTE,read);
	cpu.esi += DATA_BYTE;
	cpu.edi += DATA_BYTE;
	print_asm("movs" str(SUFFIX));
	return 1;
}


#include<cpu/exec/template-end.h>


