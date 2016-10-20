#include<cpu/exec/template-start.h>

#define instr lods
make_helper(concat(lods_,SUFFIX)){
	cpu.eax = (DATA_TYPE)cpu.esi;
	cpu.esi += DATA_BYTE;
	print_asm("lods" str(SUFFIX));
	return 1;
}


#include<cpu/exec/template-end.h>


