#include<cpu/exec/template-start.h>

#if DATA_BYTE == 2 || DATA_BYTE == 4
#define instr movsb
static void do_execute(){
	DATA_TYPE_S temp = (int8_t)op_src->val;
	OPERAND_W(op_dest,temp);
	print_asm_template2();
}
make_instr_helper(rm2r);
#undef instr
#endif

#if DATA_BYTE == 4
#define instr movsw
static void do_execute(){
	DATA_TYPE_S temp = (int16_t)op_src->val;
	OPERAND_W(op_dest,temp);
	print_asm_template2();
}
make_instr_helper(rm2r);
#endif
#include<cpu/exec/template-end.h>


