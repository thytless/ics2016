#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE_S temp = (op_src->size == 8) ? (int8_t)op_src->val : op_src->val;
	DATA_TYPE result = temp & op_dest->val;
	OPERAND_W(op_dest, result);

	cpu.eflags._cf = 0;
	cpu.eflags._of = 0;
	cpu.eflags._zf = result ? 0 : 1;
	cpu.eflags._sf = result >> (DATA_BYTE * 8 - 1);
	int n = 0,i;
	for(i = 0;i < 8;i++,result = result >> 1)
		if(result & 0x1)
			n++;
	cpu.eflags._pf = (n % 2) ? 1 : 0;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
