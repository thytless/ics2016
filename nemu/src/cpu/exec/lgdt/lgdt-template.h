#include "cpu/exec/template-start.h"

#define instr lgdt

make_helper(lgdt){
	uint32_t addr = swaddr_read(eip + 3,4);
	Log("0x%x",addr);
	cpu.GDTR.gdtLimit = swaddr_read(addr,2);
	cpu.GDTR.gdtBase = swaddr_read(addr + 2,4);
	print_asm("lgdt 0x%x ", addr);
	return 7;
}


#include "cpu/exec/template-end.h"
