#include "cpu/exec/template-start.h"
#ifndef _POP_R_
#define instr pop
static void do_execute(){
	op_dest->type = OP_TYPE_REG;
	op_dest->reg = R_ESP;
	op_dest->val = REG(R_ESP);

	uint32_t read = swaddr_read(op_dest->val,4);
	OPERAND_W(op_src,read);
	uint32_t temp = op_dest->val + 4;
	OPERAND_W(op_dest,temp);
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
