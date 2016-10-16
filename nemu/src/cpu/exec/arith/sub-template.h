#include<cpu/exec/template-start.h>

#define instr sub
/*IF	SRC is a byte and DEST is a word or dword
 *THEN	DEST -= signextend(SRC);
 *ELSE	DEST -= SRC;
 *FI
*/
static void do_execute(){
	if(op_src->type == 1){
		int32_t temp = op_src->val;
		temp = op_dest->val - temp;
		OPERAND_W(op_dest,temp);
	}
	else{
		uint32_t utemp = op_src->val;
		utemp = op_dest->val - utemp;
		OPERAND_W(op_dest,utemp);
	}	
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm);
#endif
make_instr_helper(i2a);
make_instr_helper(i2rm);
make_instr_helper(r2rm);
make_instr_helper(rm2r);

#include<cpu/exec/template-end.h>


