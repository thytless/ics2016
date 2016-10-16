#include<cpu/exec/template-start.h>

#define instr jz

static void do_execute(){
	int32_t disp = (DATA_TYPE_S)op_src->val;
	if(cpu.eflags._zf)
		cpu.eip += disp;
	print_asm_template1();
}

make_instr_helper(i);

#include<cpu/exec/template-end.h>


