#include "cpu/exec/template-start.h"

#define instr ljmp

make_helper(ljmp){
	cpu.cs.ss.val = swaddr_read(eip + 5,2);
	cpu.eip = swaddr_read(eip + 1,4);
	print_asm("ljmp 0x%x ", cpu.eip);
	cpu.eip -= 7;
	return 7;
}


#include "cpu/exec/template-end.h"
