#include<cpu/exec/template-start.h>

#define instr setnz

static void do_execute(){
	uint8_t temp = cpu.eflags._zf ? 0 : 1;
	OPERAND_W(op_src,temp);
	print_asm_template1();
}

make_instr_helper(rm);


#include<cpu/exec/template-end.h>


