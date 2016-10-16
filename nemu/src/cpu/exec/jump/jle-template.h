#include<cpu/exec/template-start.h>

#define instr jle

static void do_execute(){
	DATA_TYPE disp = op_src->val;
	assert(disp < 0);
	if(cpu.eflags._of != cpu.eflags._sf || cpu.eflags._zf)
		cpu.eip += disp;
	print_asm_template1();
}

make_instr_helper(i);

#include<cpu/exec/template-end.h>


