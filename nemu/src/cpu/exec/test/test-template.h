#include<cpu/exec/template-start.h>

#define instr test

static void do_execute(){
	cpu.eflags._cf = 0;
	cpu.eflags._of = 0;
	uint32_t test = op_src->val & op_dest->val;
	cpu.eflags._zf = test ? 0 : 1;
	int n = 0, i;
	for(i = 0;i < 8;i++,test = test >> 1)
		if(test & 0x1)
			n++;
	cpu.eflags._pf = (n % 2 == 0) ? 1 : 0;
	cpu.eflags._sf = ((test >> 23) & 0x1) ? 1 : 0;
	print_asm_template2();
}

make_instr_helper(r2rm);
make_instr_helper(i2rm);
make_instr_helper(i2a);


#include<cpu/exec/template-end.h>


