#include "cpu/exec/template-start.h"

make_helper(ret_near){
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_ESP;
	op_src->val = REG(R_ESP);

	cpu.eip = swaddr_read(op_src->val,4) - 1;
	uint32_t temp = op_src->val + 4;
	OPERAND_W(op_src,temp);
	print_asm("ret");
	return 1;
}

#include "cpu/exec/template-end.h"
