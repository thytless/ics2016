#include<cpu/exec/template-start.h>

#define instr jnz

static void do_execute(){
	if(!cpu.eflags._zf)
		cpu.eip += op_src->val;
	print_asm_template1();
}

make_instr_helper(i);

#include<cpu/exec/template-end.h>


