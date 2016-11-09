#include <stdio.h>
#include <stdint.h>
#include "FLOAT.h"
#include <sys/mman.h>
#include <assert.h>

extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);
extern char _ppfs_setargs;

__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f) {
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */
	
	
	char buf[80];
	char *p = buf;
	int sign = (f >> 0x1f) & 0x1;
	int len = 0;
	if(sign){
		len += sprintf(p, "-");
		f = -f;
		++p;
	}
	int length = sprintf(p, "%d.", (f >> 16) & 0xfff);

	len += length;
	p += length;

	long long frag = f & 0xffff;

	frag = (frag * 1000000) >> 16;

	int divc = 0;

	int fr = frag;
	while(fr > 10){
		fr /= 10;
		++divc;
	}
	sprintf(p, "%d", fr);
	p++;

	int i = 1;
	for(;i < 6;i++){
		int j;
		fr = frag;
		for(j = 0;j < divc - i;j++)
			fr /= 10;
		sprintf(p, "%d", (fr % 10));
		p++;
	}

//	int len = sprintf(buf, "0x%08x", f);
	return __stdio_fwrite(buf, len + 6, stream);
}

static void modify_vfprintf() {
	/* TODO: Implement this function to hijack the formating of "%f"
	 * argument during the execution of `_vfprintf_internal'. Below
	 * is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

	
	uint8_t *call_addr =(uint8_t *)((uint32_t)&_vfprintf_internal + 0x306);
	int32_t disp = (int)format_FLOAT - (int)&_fpmaxtostr;

//	mprotect((void *)((int)(call_addr - 100) & 0xfffff000),4096 * 2,PROT_READ | PROT_WRITE | PROT_EXEC);

	/* STEP 1: modify call displacement */
	*(uint32_t *)(call_addr + 1) += disp;

	/* STEP 2: modify arguments */
																	// sub $0xc,%esp		83 ec 0c
	uint8_t *arg1_addr = (uint8_t *)call_addr + (0x4a - 0x54);		// fstpt (%esp)			db 3c 24	->	push (%edx) ff 32 90
	uint8_t *nop_addr = arg1_addr + 2;								// 0x24 -> 0x90		
	uint8_t *arg2_addr = nop_addr + 1;								// pushl 0x18c(%esp)	ff b4 24 8c 01 00 00

	*arg1_addr = 0xff;
	*(arg1_addr + 1) = 0x32;
	*nop_addr = 0x90;
	*(arg1_addr - 1) = 0x8;

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

}

static void modify_ppfs_setargs() {
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */
	
	uint8_t *jmp_addr = (uint8_t *)((uint32_t) &_ppfs_setargs + (0x23d - 0x1ce));
/*
	 replace float instr with nop 
	uint8_t *f_instr[4] = {
		(uint8_t *)((uint32_t) &_ppfs_setargs + (0x26a - 0x1ce)),
		(uint8_t *)((uint32_t) &_ppfs_setargs + (0x26f - 0x1ce)),
		(uint8_t *)((uint32_t) &_ppfs_setargs + (0x28d - 0x1ce)),
		(uint8_t *)((uint32_t) &_ppfs_setargs + (0x295 - 0x1ce))
	};
*/
	/* 0x23d + 2 + disp = 0x271 */
	
	int8_t disp = 0x32;
	
	
//	mprotect((void *)(((int)&_ppfs_setargs) & 0xfffff000),32768,PROT_READ | PROT_WRITE | PROT_EXEC);

	/* 804823d:    74 32		je 8049271*/
	*jmp_addr = 0x74;
	*(jmp_addr + 1) = disp;
/*	
	int i = 0;
	printf("%x\n",(int)&_ppfs_setargs);
	printf("%x\n",(int)&_ppfs_setargs & 0xfffff000);
	for(;i < 4;i++){
		printf("%x\n",(int)f_instr[i]);
		*f_instr[i] = 0x90;
		*(f_instr[i] + 1) = 0x90;
	}
*/


#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):				/* 0x0800 */
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;	
			case (PA_INT|PA_FLAG_LONG):						/* 0x0400 */
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */			/* 0x0001 */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
															/* 0x0100 */
			case (PA_INT|PA_FLAG_SHORT):					/* 0x0200 */
			case PA_INT:									/* 0x0000 */
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */		/* 0x0002 */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:									/* 0x0007 */
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):			/* 0x0807 */
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif

}

void init_FLOAT_vfprintf() {
	modify_vfprintf();
	modify_ppfs_setargs();
}
