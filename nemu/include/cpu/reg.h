#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "../../lib-common/x86-inc/cpu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */
/* the Segment Selector */
typedef union {
	struct{
		unsigned _rpl:2;
		unsigned _ti:1;
		unsigned _index:13;
	};
	uint16_t val;
}SegSel;

/* the Segment Register */
typedef struct {
	SegDesc sd;
	SegSel ss;
}SReg;

/* the CPU */
typedef struct {
	union{
		union {
			uint32_t _32;
			uint16_t _16h;
			uint16_t _16;
			uint16_t _8h;
			uint8_t _8[2];

		} gpr[8];
	
	/* Do NOT change the order of the GPRs' definitions. */
		struct{
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	swaddr_t eip;
	union{
		struct{			
			unsigned _cf:1;
			unsigned _s1:1;//set
			unsigned _pf:1;
			unsigned _r3:1;//reset

			unsigned _af:1;
			unsigned _r2:1;//reset
			unsigned _zf:1;
			unsigned _sf:1;
			
			unsigned _tf:1;
			unsigned _if:1;
			unsigned _df:1;
			unsigned _of:1;
			
			uint32_t _r1:20;//reset
		};
		uint32_t val;
	} eflags;
	
	struct{
		uint16_t gdtLimit;
		lnaddr_t gdtBase;
	} GDTR;

	SReg cs,ss,ds,es;
	CR0 cr0;
	CR3 cr3;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
