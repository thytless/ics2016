#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args){
	
	return 0;
}

static int cmd_info(char *args){
	if(strcmp(args,"w") == 0){
		printf("info: w: Command under Construction\n");
	}
	else if(strcmp(args,"r") == 0){
		extern CPU_state cpu;
		printf("eax\t\t %x\t %d\n",cpu.eax,cpu.eax);
		printf("ecx\t\t %x\t %d\n",cpu.ecx,cpu.ecx);
		printf("edx\t\t %x\t %d\n",cpu.edx,cpu.edx);
		printf("ebx\t\t %x\t %d\n",cpu.ebx,cpu.ebx);
		printf("esp\t\t %x\t\n",cpu.esp);
		printf("ebp\t\t %x\t\n",cpu.ebp);
		printf("esi\t\t %x\t %d\n",cpu.esi,cpu.esi);
		printf("edi\t\t %x\t %d\n",cpu.edi,cpu.edi);
		printf("eip\t\t %x\t\n",cpu.eip);
	}
	else{
		printf("%s: No such subcmd\n",args);
	}
	return 0;
}

static int cmd_p(char *args){
	
	return 0;
}

static int cmd_x(char *args){

	return 0;
}

static int cmd_w(char *args){

	return 0;
}

static int cmd_d(char *args){

	return 0;
}

static int cmd_bt(char *args){

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
	{ "bt", "Print the stack frame" , cmd_bt },//PA2
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
