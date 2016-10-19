#include<cpu/exec/template-start.h>

#define instr jmp

make_helper(concat(jmp_i_,SUFFIX)){
	int len = concat(decode_i_,SUFFIX)(eip + 1);
	int32_t disp = (DATA_TYPE_S)op_src->val;
	cpu.eip += disp;
	print_asm("jmp" str(SUFFIX) " %x",op_src->val);
	return len + 1;
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_rm_,SUFFIX)){
	int len = concat(decode_rm_,SUFFIX)(eip + 1);
	cpu.eip = (DATA_BYTE == 2) ? 0x0000FFFF & op_src->val : op_src->val;
	print_asm("jmp" str(SUFFIX) " %d",cpu.eip);
	return len + 1;
}
#endif

#include<cpu/exec/template-end.h>


