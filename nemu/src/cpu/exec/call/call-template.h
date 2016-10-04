#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	//push eip
	//that is
	//esp -= 4
	//*esp = eip
	op_dest->type = OP_TYPE_REG;
	op_dest->reg = R_ESP;
	op_dest->val = REG(R_ESP);
	snprintf(op_dest->str, OP_STR_SIZE, "%s",REG_NAME(R_ESP));
	
	uint32_t temp = op_dest->val - 4;
	swaddr_write(temp,4,cpu.eip);
	OPERAND_W(op_dest,temp);
	
	cpu.eip += op_src->val;
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(call_i_,SUFFIX)){
	int len = concat(decode_i_,SUFFIX)(eip + 1);
	cpu.eip += len + 1;
	do_execute();
	return 0;
}
#endif

#include "cpu/exec/template-end.h"
