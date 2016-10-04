#include<cpu/exec/template-start.h>

#define instr cmp

static void do_execute(){
	int sub = op_src->val - op_dest->val;
	cpu.eflags._zf = sub ? 0 : 1;
	cpu.eflags._cf = (sub >= 0) ? 0 : 1;
	cpu.eflags._sf = cpu.eflags._cf;
	if(op_src->val > 0 && op_dest->val < 0 && sub < 0)
		cpu.eflags._of = 1;
	else if(op_src->val < 0 && op_dest->val > 0 && sub > 0)
		cpu.eflags._of = 1;
	else
		cpu.eflags._of = 0;
	int n = 0,i;
	for(i = 0;i < 8;i++,sub = sub >> 1)
		if(sub & 0x1)
			n++;
	cpu.eflags._pf = (n % 2) ? 1 : 0;
}

make_instr_helper(si2rm);

#include<cpu/exec/template-end.h>


