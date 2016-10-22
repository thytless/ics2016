#include "cpu/exec/template-start.h"
#ifndef _PUSH_R_
#define instr push
static void do_execute(){
	int disp = (DATA_BYTE == 2) ? 2 : 4;
	cpu.esp -= disp;
//	swaddr_write(cpu.esp,disp,(DATA_TYPE_S)op_src->val);
	swaddr_write(cpu.esp,disp,op_src->val);
	print_asm_template1();
}
make_instr_helper(i);
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm);
#endif
#else
make_helper(concat4(push_,regname,_,SUFFIX)){
/*
	op_src->type = OP_TYPE_REG;
	op_src->reg = SRC_REG;
	op_src->val = REG(SRC_REG);

	do_execute();
	*/
	uint32_t temp = cpu.esp - 4;
	swaddr_write(temp,4,REG(SRC_REG));
	cpu.esp -= 4;
	print_asm_template1();
	
	print_asm("push" str(SUFFIX) " %%%s",REG_NAME(SRC_REG));

	return 1;
}
#endif


#include "cpu/exec/template-end.h"
