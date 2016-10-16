#include<cpu/exec/template-start.h>

#define instr idiv

make_helper(concat(idiv_rm2a_,SUFFIX)){
	int len = concat(decode_rm_imm_,SUFFIX)(eip);
	assert(op_src->val);

	DATA_TYPE_S divisor = op_src->val;
	assert(divisor);

#if DATA_BYTE == 8
	int16_t dividend = REG(R_AX);
	op_dest->reg = R_AL;
	op_dest->val = REG(R_AL);	
	DATA_TYPE_S temp = dividend / divisor;
	DATA_TYPE_S rem = dividend % divisor;
	OPERAND_W(op_dest,temp);
	op_dest->reg = R_AH;
	op_dest->val = REG(R_AH);
	OPERAND_W(op_dest,rem);
#endif

#if DATA_BYTE == 16
	int32_t dividend = REG(R_DX);
	dividend = dividend << 16 + REG(R_AX);
	op_dest->reg = R_AX;
	op_dest->val = REG(R_AX);	
	DATA_TYPE_S temp = dividend / divisor;
	DATA_TYPE_S rem = dividend % divisor;
	OPERAND_W(op_dest,temp);
	op_dest->reg = R_DX;
	op_dest->val = REG(R_DX);
	OPERAND_W(op_dest,rem);
#endif

#if DATA_BYTE == 32
	int64_t dividend = REG(R_EDX);
	dividend = dividend << 32 + REG(R_EAX);
	op_dest->reg = R_EAX;
	op_dest->val = REG(R_EAX);	
	DATA_TYPE_S temp = dividend / divisor;
	DATA_TYPE_S rem = dividend % divisor;
	OPERAND_W(op_dest,temp);
	op_dest->reg = R_EDX;
	op_dest->val = REG(R_EDX);
	OPERAND_W(op_dest,rem);
#endif				

	print_asm_template1();
	return len + 2;
}

#include<cpu/exec/template-end.h>


