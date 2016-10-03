#include "cpu/exec/template-start.h"

#define instr push


#ifdef _PUSH_R_
make_helper(concat(push_,regname)){
	op_src->type = OP_TYPE_REG;
	op_src->reg = SRC_REG;
	op_src->val = REG(SRC_REG);
	
	op_dest->type = OP_TYPE_REG;
	op_dest->reg = R_ESP;
	op_dest->val = REG(R_ESP);

	uint32_t temp = op_dest->val - 4;
	swaddr_write(temp,4,op_src->val);
	OPERAND_W(op_dest,temp);

	return 1;
}
#endif



#include "cpu/exec/template-end.h"
