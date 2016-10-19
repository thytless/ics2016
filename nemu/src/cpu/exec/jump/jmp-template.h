#include<cpu/exec/template-start.h>

#define instr jmp

static void do_execute(){
	int32_t temp = (DATA_TYPE_S)op_src->val;
	switch(ops_decoded.opcode){
		case 0xeb:case 0xe9:
			cpu.eip += temp;break;
		case 0xFF:
			cpu.eip = temp;
	}
	print_asm("jmp" str(SUFFIX) " %x",cpu.eip);
}

make_instr_helper(i);
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm);
#endif

#include<cpu/exec/template-end.h>


