#include "cpu/exec/template-start.h"

#define instr ljmp

make_helper(ljmp){
	uint32_t addr = swaddr_read(eip + 2,4);
	Log("0x%x",addr);
	cpu.cs.ss.val = swaddr_read(addr,2);
	cpu.eip = swaddr_read(addr + 2,4);
	print_asm("ljmp 0x%x ", addr);
	return 7;
}


#include "cpu/exec/template-end.h"
