#include<cpu/exec/template-start.h>

#define instr scas
make_helper(concat(scas_,SUFFIX)){
	DATA_TYPE s = (DATA_TYPE)cpu.eax;
	DATA_TYPE d = (DATA_TYPE)cpu.edi;
	DATA_TYPE sub = s - d;
	
	cpu.eflags._zf = sub ? 0 : 1;
	cpu.eflags._sf = sub >> (DATA_BYTE * 8 - 1);
	cpu.eflags._cf = (s < d);
	bool src_sign = s >> (DATA_BYTE * 8 - 1);
	bool dest_sign = d >> (DATA_BYTE * 8 - 1);
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
	print_asm("scas" str(SUFFIX));
	return 1;
}


#include<cpu/exec/template-end.h>


