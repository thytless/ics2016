#include "cpu/exec/template-start.h"

#define instr pop


#ifdef _POP_R_
make_helper(concat(pop_,regname)){
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_ESP;
	op_src->val = REG(R_ESP);
	
	op_dest->type = OP_TYPE_REG;
	op_dest->reg = DEST_REG;
	op_dest->val = REG(DEST_REG);

	uint32_t read = swaddr_read(op_src->val,4);
	OPERAND_W(op_dest,read);
	uint32_t temp = op_src->val + 4;
	OPERAND_W(op_src,temp);

	return 1;
}
#endif



#include "cpu/exec/template-end.h"
