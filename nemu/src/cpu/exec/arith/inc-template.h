#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE sum = op_src->val + 1;
	OPERAND_W(op_src, sum);

	/* TODO: Update EFLAGS. */

	cpu.eflags._zf = sum ? 0 : 1;
	cpu.eflags._sf = sum >> (DATA_BYTE * 8 - 1);

	if(op_src->val > 0 && sum < op_src->val)
		cpu.eflags._cf = 1;
	else
		cpu.eflags._cf = 0;

	bool src_sign = op_src->val >> (op_src->size * 8 - 1);
	if(!src_sign && cpu.eflags._sf)
		cpu.eflags._of = 1;
	else
		cpu.eflags._of = 0;
	int n = 0,i;
	for(i = 0;i < 8;i++,sum = sum >> 1)
		if(sum & 0x1)
			n++;
	cpu.eflags._pf = (n % 2) ? 1 : 0;

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
