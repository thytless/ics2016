#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <elf.h>

#define MAX_INSTR_TO_PRINT 100000
void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	} 

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	} 

	return line_read;
}

static int cmd_c(char *args) {
	printf("Continuing.");
	cpu_exec(-1);

	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args){
	if(args == NULL){
		cpu_exec(1);
		return 0;
	} 
	int steps = atoi(args);
	if(steps <= 0 || steps >= MAX_INSTR_TO_PRINT){
		printf("si: Invalid step input(should be 1 ~ 9)\n");
	}
	else{
		cpu_exec(steps);
	}
	return 0;
}

static int cmd_info(char *args){
	if(args == NULL){
		printf("info: Lack of arguments.Try 'help info'\n");
		return 0;
	}
	if(strcmp(args,"w") == 0){
		WP *info = get_head();
		if(info == NULL){
			printf("No working watchpoints\n");
			return 0;
		}
		printf("Watchpoint\tvalue\t\texpr\n");
		while(info != NULL){
			printf("    %d\t\t%d\t\t%s\n",info->NO,info->old_value,info->expr);
			info = info -> next;
		}
		return 0;
//		printf("info: w: Command under Construction\n");
	}
	else if(strcmp(args,"r") == 0){
		extern CPU_state cpu;
		int tof, tdf, tif, tsf, tzf, tpf, tcf;
		tof = cpu.eflags._of;
		tdf = cpu.eflags._df;
		tif = cpu.eflags._if;
		tsf = cpu.eflags._sf;
		tzf = cpu.eflags._zf;
		tpf = cpu.eflags._pf;
		tcf = cpu.eflags._cf;
		printf("eax\t\t 0x%x\t %d\n",cpu.eax,cpu.eax);
		printf("ecx\t\t 0x%x\t %d\n",cpu.ecx,cpu.ecx);
		printf("edx\t\t 0x%x\t %d\n",cpu.edx,cpu.edx);
		printf("ebx\t\t 0x%x\t %d\n",cpu.ebx,cpu.ebx);
		printf("esp\t\t 0x%x\t\n",cpu.esp);
		printf("ebp\t\t 0x%x\t\n",cpu.ebp);
		printf("esi\t\t 0x%x\t %d\n",cpu.esi,cpu.esi);
		printf("edi\t\t 0x%x\t %d\n",cpu.edi,cpu.edi);
		printf("eip\t\t 0x%x\t\n",cpu.eip);
		printf("eflags\t\t 0x%x\t\n",cpu.eflags.val);
		printf("------------------------------------\n");
		printf("| OF | DF | IF | SF | ZF | PF | CF |\n");
		printf("|  %d |  %d |  %d |  %d |  %d |  %d |  %d |\n",
				 tof, tdf, tif, tsf, tzf, tpf, tcf);
		printf("------------------------------------\n");

	}
	else{
		printf("%s: No such subcmd\n",args);
	}
	return 0;
}

static int cmd_p(char *args){
	bool success = true;
	int value;
	value = expr(args,&success);
	if(value >= 0xffff)
		printf("%s = 0x%X\n",args,value);
	else
		printf("%s = %d\n",args,value);
	if(!success)
		panic("Making tokens failed\n");
	return 0;
}

static int cmd_x(char *args){
	if(args == NULL){
		printf("x: Lack of arguments\n");
		return 0;
	} 
	size_t len = atoi(strtok(args," "));
	char *address = strtok(NULL," ");
 	if(address == NULL || address[0] == '\0'){
		printf("x: Lack of arguments\n");
		return 0;
	}
	bool success;
	uint32_t addr = (uint32_t)expr(address,&success);
	int i = 0;
		for(;i < len;i++){
			uint32_t temp = swaddr_read(addr + i * sizeof(uint32_t),sizeof(uint32_t));
			if(temp >= 0xffff)
				printf("0x%x : \t 0x%x\n",addr + i * sizeof(uint32_t),temp);
			else
				printf("0x%x : \t %d\n",addr + i * sizeof(uint32_t),temp);
	}
	return 0;
}

static int cmd_w(char *args){
	WP *nwp = new_wp();
	bool success;
	strcpy(nwp -> expr,args);
	if((nwp -> old_value = expr(args,&success)) < 0xffff)
		printf("Watchpoint %d: %s = %d \n",nwp -> NO,args,nwp -> old_value);
	else
		printf("Watchpoint %d: %s = 0x%X \n",nwp -> NO,args,nwp -> old_value);
	return 0;
}

static int cmd_d(char *args){
	int n = atoi(args);
	if(n >= 0 && n <= 31){
		WP* wp = get_head();
		while(wp != NULL){
			if(wp -> NO == n){
				free_wp(wp);
				printf("Watchpoint %d: %s deleted\n",wp -> NO,wp -> expr);
				break;
			}
			wp = wp -> next;
		}
		if(wp == NULL)
			printf("Watchpoint %d is not working\n",n);
	}
	else
		printf("Invalid argument : %s\n",args);
	return 0;
}

typedef struct partofStackFrame{
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args;
}SF;

char *getStrtab();
Elf32_Sym *getSymtab();
int getSymtableEntry();

static int cmd_bt(char *args){
	/* print ebp, function name and first four args */
	
	char *strtab = getStrtab();
	Elf32_Sym *symtab = getSymtab();
	int entry = getSymtableEntry();

	swaddr_t ebp = cpu.ebp;
	swaddr_t eip = cpu.eip;
	int i = 1;
	do{
		SF sf;
		sf.args = ebp + 8;

		int k = 0;
		char *name;
		for(k = 0;k < entry;k++){
			if((symtab[k].st_info & 0xf) == STT_FUNC){
				int start = symtab[k].st_value;
				int end = symtab[k].st_value + symtab[k].st_size;
				if(eip >= start && eip <= end){
					name = symtab[k].st_name + strtab;
					break;
				}
			}
		}
		if(i)
			printf("#%d  0x%x in %s \(",i,eip,name);
		else
			printf("#%d  %s (",i,name);

		printf("  ");
		int j = 0;
		for(;j < 4;j++){
			uint32_t arg = swaddr_read(sf.args + 4*j,4);
			if(arg > 0xffff)
				printf("$%d = 0x%x  ",j,arg);
			else
				printf("$%d = %d  ",j,arg);
		}

		printf(")\n");

		i++;
		/* return to old function */

		sf.prev_ebp = swaddr_read(ebp,4);
		sf.ret_addr = ebp + 4;
		ebp = sf.prev_ebp;
		eip = swaddr_read(sf.ret_addr,4);
		if(!ebp)
			break;

	}while(1);
	return 0;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Single step execution", cmd_si }, //Stage 1
	{ "info", "Print the running state of the program", cmd_info }, //Stage 1
	{ "p", "Evaluate the expression" , cmd_p }, //Stage 2
	{ "x", "Scan memory" , cmd_x }, //Stage 1 (simplified)
	{ "w", "Set a watchpoint" , cmd_w }, //Stage 3
	{ "d", "Delete a watchpoint" , cmd_d }, //Stage 3
	{ "bt", "Print the stack frame chain" , cmd_bt },//PA2
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
 	}
 	else {
 		for(i = 0; i < NR_CMD; i ++) {
 			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
