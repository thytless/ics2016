#include "cpu/exec/template-start.h"

#define instr lgdt

make_helper(lgdt){
	uint32_t addr = swaddr_read(eip + 1,4);
	cpu.GDTR.gdtLimit = swaddr_read(addr,2);
	cpu.GDTR.gdtBase = swaddr_read(addr + 2,4);
	print_asm("lgdt 0x%x ", addr);
	return 5;
}


#include "cpu/exec/template-end.h"
