#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */


typedef struct {
	struct {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];

	} gpr[8];
	
	/* Do NOT change the order of the GPRs' definitions. */
	union {
		uint32_t eax;
		uint16_t hax;
		uint16_t ax;
		uint16_t ha;
		uint8_t ah;
		uint8_t al;
	} ;
	union {
		uint32_t ecx;
		uint16_t hcx;
		uint16_t cx;
		uint16_t hc;
		uint8_t ch;
		uint8_t cl;
	} ;
	union {
		uint32_t edx;
		uint16_t hdx;
		uint16_t dx;
		uint16_t hd;
		uint8_t dh;
		uint8_t dl;
	} ;
	union {
		uint32_t ebx;
		uint16_t hbx;
		uint16_t bx;
		uint16_t hb;
		uint8_t bh;
		uint8_t bl;
	} ;
	union {
		uint32_t ebp;
		uint16_t hbp;
		uint16_t bp;
	} ;
	union {
		uint32_t esp;
		uint16_t hsp;
		uint16_t sp;
	} ;
	union {
		uint32_t esi;
		uint16_t hsi;
		uint16_t si;
	} ;
	union {
		uint32_t edi;
		uint16_t hdi;
		uint16_t di;
	} ;
	//uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

	swaddr_t eip;

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
