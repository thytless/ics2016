#include<cpu/exec/template-start.h>

#define instr cltd

make_helper(cltd){
	if(!(REG(R_EAX)&0xFFFF0000)){
		op_dest->type = OP_TYPE_REG;
		op_dest->reg = R_DX;
		op_dest->val = REG(R_DX);
		if(REG(R_AX) < 0)
			OPERAND_W(op_dest,0xFFFF);	
		else
			OPERAND_W(op_dest,0);
	print_asm("cwtl");	
	}
	else{
		op_dest->type = OP_TYPE_REG;
		op_dest->reg = R_EDX;
		op_dest->val = REG(R_EDX);
		if(REG(R_EAX) < 0)
			OPERAND_W(op_dest,0xFFFFFFFF);	
		else
			OPERAND_W(op_dest,0);
	print_asm("cltd");
	}
	return 1;
}
#include<cpu/exec/template-end.h>


