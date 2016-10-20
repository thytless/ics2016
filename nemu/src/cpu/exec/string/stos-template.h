#include<cpu/exec/template-start.h>

#define instr stos
make_helper(concat(stos_,SUFFIX)){
	swaddr_write(cpu.edi,DATA_BYTE,(DATA_TYPE)cpu.eax);
	cpu.edi += DATA_BYTE;
	print_asm("stos" str(SUFFIX));
	return 1;
}


#include<cpu/exec/template-end.h>


