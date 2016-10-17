#include<cpu/exec/template-start.h>

#define instr sbb
/*IF	SRC is a byte and DEST is a word or dword
 *THEN	DEST -= signextend(SRC);
 *ELSE	DEST -= SRC;
 *FI
*/
static void do_execute(){
	DATA_TYPE_S temp = (op_src->size == 8) ? (int8_t)op_src->val : op_src->val;
	DATA_TYPE_S sub = op_dest->val - temp - cpu.eflags._cf;
	OPERAND_W(op_dest,sub);

	cpu.eflags._zf = sub ? 0 : 1;
	cpu.eflags._cf = (sub >= 0) ? 0 : 1;
	cpu.eflags._sf = cpu.eflags._cf;
	if(op_src->val > 0 && op_dest->val < 0 && sub < 0)
		cpu.eflags._of = 1;
	else if(op_src->val < 0 && op_dest->val > 0 && sub > 0)
		cpu.eflags._of = 1;
	else
		cpu.eflags._of = 0;
	int n = 0,i;
	for(i = 0;i < 8;i++,sub = sub >> 1)
		if(sub & 0x1)
			n++;
	cpu.eflags._pf = (n % 2) ? 1 : 0;
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm);
#endif
make_instr_helper(i2a);
make_instr_helper(i2rm);
make_instr_helper(r2rm);
make_instr_helper(rm2r);

#include<cpu/exec/template-end.h>

