#include "cpu/exec/template-start.h"
#ifndef _POP_R_
#define instr pop
static void do_execute(){
	int disp = (DATA_BYTE == 2) ? 2 : 4;
	uint32_t read = swaddr_read(cpu.esp,disp);
	OPERAND_W(op_src,read);
	cpu.esp += disp;
	print_asm_template1();
}
make_instr_helper(i);
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm);
#endif
#else
make_helper(concat4(pop_,regname,_,SUFFIX)){
	
	op_src->type = OP_TYPE_REG;
	op_src->reg = DEST_REG;
	op_src->val = REG(DEST_REG);

	do_execute();
	print_asm("pop" str(SUFFIX) " %%%s", REG_NAME(DEST_REG));
	return 1;
}
#endif



#include "cpu/exec/template-end.h"
