#include "cpu/exec/template-start.h"

#define instr leave


make_helper(leave){
	op_dest->type = OP_TYPE_REG;
	op_dest->reg = R_ESP;
	op_dest->val = REG(R_ESP);

	op_src->type = OP_TYPE_REG;
	op_src->reg = R_EBP;
	op_src->val = REG(R_EBP);
	assert(op_src->val == 0x7ffffe8);

	OPERAND_W(op_dest,op_src->val);
	assert(op_dest->val == 0x7ffffe8);

	uint32_t temp = swaddr_read(op_dest->val,4);
	OPERAND_W(op_src,temp);
	temp = op_dest->val + 4;
	OPERAND_W(op_dest,temp);
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
