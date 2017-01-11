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
make_helper(mov_r2cr){
	uint8_t modrm = swaddr_read(eip + 1,1);
	int cr = (modrm >> 3) & 0x7;
	int r = modrm & 0x7;
	switch(cr){
		case 0:cpu.cr0.val = REG(r);break;
		default: Log("error cr number!");
	}
	print_asm("mov %%%s,%%cr%d", REG_NAME(r), cr);
	return 2;
}

make_helper(mov_cr2r){
	uint8_t modrm = swaddr_read(eip + 1,1);
	int cr = (modrm >> 3) & 0x7;
	int r = modrm & 0x7;
	switch(cr){
		case 0: REG(r) = cpu.cr0.val;break;
		default: Log("error cr number!");
	}
	print_asm("mov %%cr%d,%%%s", cr, REG_NAME(r));
	return 2;
}
#endif

#if DATA_BYTE == 2
make_helper(mov_r2sr_w){
	uint8_t modrm = swaddr_read(eip + 1,1);
	int r = modrm & 0x7;
	int seg = (modrm >> 3) & 0x7;
	SReg *sreg;
	switch(seg){
		case 0:sreg = &cpu.es;break;
		case 2:sreg = &cpu.ss;break;
		case 3:sreg = &cpu.ds;break;
		default:Log("Bad SReg!");return 2;
	}
	sreg->ss.val = (REG(r) & 0xffff);
	uint16_t index = sreg->ss._index;
	lnaddr_t addr = cpu.GDTR.gdtBase + sizeof(SegDesc) * index;
	sreg->sd.limit_15_0 = lnaddr_read(addr,2);
	sreg->sd.base_15_0 = lnaddr_read(addr+2,2);
	sreg->sd.base_23_16 = lnaddr_read(addr+4,1);
	sreg->sd.limit_19_16 = lnaddr_read(addr+6,1) & 0xf;
	sreg->sd.base_31_24 = lnaddr_read(addr+7,1);
	return 2;
}
#endif

#include "cpu/exec/template-end.h"
