#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4
make_helper(mov_r_cr){
	uint8_t modrm = swaddr_read(eip + 1,1);
	int cr = (modrm >> 3) & 0x7;
	int r = modrm & 0x7;
	switch(cr){
		case 0:cpu.cr0.val = REG(r);
		default: Log("error cr number!");
	}
	print_asm("mov %%%s,%%cr%d", REG_NAME(r), cr);
	return 3;
}

make_helper(mov_cr_r){
	uint8_t modrm = swaddr_read(eip + 1,1);
	int cr = (modrm >> 3) & 0x7;
	int r = modrm & 0x7;
	switch(cr){
		case 0: REG(r) = cpu.cr0.val;
		default: Log("error cr number!");
	}
	print_asm("mov %%cr%d,%%%s", cr, REG_NAME(r));
	return 3;
}
#endif

#include "cpu/exec/template-end.h"
