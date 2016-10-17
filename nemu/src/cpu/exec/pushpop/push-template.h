#include "cpu/exec/template-start.h"
#ifndef _PUSH_R_
#define instr push
static void do_execute(){
	op_dest->type = OP_TYPE_REG;
	op_dest->reg = R_ESP;
	op_dest->val = REG(R_ESP);

	uint32_t temp = op_dest->val - 4;
	swaddr_write(temp,4,op_src->val);
	OPERAND_W(op_dest,temp);
}
make_instr_helper(i);
#else
make_helper(concat4(push_,regname,_,SUFFIX)){

	op_src->type = OP_TYPE_REG;
	op_src->reg = SRC_REG;
	op_src->val = REG(SRC_REG);

	do_execute();
	print_asm("push" str(SUFFIX) " %%%s",REG_NAME(SRC_REG));

	return 1;
}
#endif


#include "cpu/exec/template-end.h"
