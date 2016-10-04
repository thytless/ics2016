#include<cpu/exec/template-start.h>

#define instr jz

static void do_execute(){
	if(cpu.eflags._zf)
		cpu.eip += op_src->val;
}

make_instr_helper(i);

#include<cpu/exec/template-end.h>


