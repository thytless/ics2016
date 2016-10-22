#include<cpu/exec/template-start.h>

#if DATA_BYTE == 2 || DATA_BYTE == 4
#define instr movzb
static void do_execute(){
	OPERAND_W(op_dest,(uint8_t)op_src->val);
	print_asm_template2();
}
make_instr_helper(rm2r);
#undef instr
#endif

#if DATA_BYTE == 4
#define instr movzw
static void do_execute(){
	OPERAND_W(op_dest,(uint16_t)op_src->val);
	print_asm_template2();
}
make_instr_helper(rm2r);
#endif
#include<cpu/exec/template-end.h>


