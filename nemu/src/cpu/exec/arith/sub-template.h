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

make_instr_helper(si2rm);



#include<cpu/exec/template-end.h>


