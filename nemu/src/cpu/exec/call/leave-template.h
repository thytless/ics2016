#include "cpu/exec/template-start.h"

#define instr leave


make_helper(leave){

	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
