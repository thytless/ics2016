#include<cpu/exec/template-start.h>

#define instr sub
/*IF	SRC is a byte and DEST is a word or dword
 *THEN	DEST -= signextend(SRC);
 *ELSE	DEST -= SRC;
 *FI
*/
static void do_execute(){
	DATA_TYPE_S temp = (op_src->size == 1) ? (int8_t)op_src->val : op_src->val;
	DATA_TYPE_S sub = op_dest->val - temp;
	OPERAND_W(op_dest,sub);

	cpu.eflags._zf = sub ? 0 : 1;
	cpu.eflags._sf = sub >> (DATA_BYTE * 8 - 1);
	cpu.eflags._sf = cpu.eflags._cf;
	bool src_sign = op_src->val >> (op_src->size * 8 - 1);
	bool dest_sign = op_dest->val >> (op_dest->size * 8 - 1);
	if(src_sign && !dest_sign && cpu.eflags._sf)
		cpu.eflags._of = 1;
	else if(!src_sign && dest_sign && !cpu.eflags._sf)
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


