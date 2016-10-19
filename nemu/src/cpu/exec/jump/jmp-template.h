#include<cpu/exec/template-start.h>

#define instr jmp

static void do_execute(){
	int32_t disp = (DATA_TYPE_S)op_src->val;
	cpu.eip += disp;
	print_asm_template1();
}

make_instr_helper(i);
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm);
#endif

#include<cpu/exec/template-end.h>


