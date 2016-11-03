#include "nemu.h"
#include "elf.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum { 
	NOTYPE, PLUS, EQ, HEX, DECI, SUB, MULT, DIV,
	LP, RP, REG32, REG16, REG8, IDE, NEQ, AND, OR, DEREF

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", PLUS},					// plus
	{"==", EQ},						// equal
	{"0[xX][0-9a-fA-F]+", HEX},	    // hex
	{"[0-9]+", DECI},				// decimal
	{"-", SUB},						// substract
	{"\\*", MULT},					// multiply
	{"/", DIV},						// divide
	{"\\(", LP},					// left bracket
	{"\\)", RP},					// right bracket
	{"\\$e(ax|bx|cx|dx|di|si|bp|sp)", REG32},
									// 32-bit register
	{"\\$(bx|cx|ax|dx|di|si|bp|sp)", REG16},
	{"\\$(ah|bh|ch|dh|al|bl|cl|dl)", REG8},
//	{"(?<=\\+|-|\\*|/|\\()\\*", DEREF},	// dereference
	{"[_a-zA-Z][_0-9a-zA-Z]*", IDE},	// identifier
	{"!=", NEQ},
	{"&&", AND},
	{"||", OR}

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];
int eval(int,int);
bool check_parentheses(int,int);
int last_token(int,int,int);
uint32_t getIDEaddr(char *);
char *getStrtab();
Elf32_Sym *getSymtab();
/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token { 
	int type;
	char str[32];
} Token; 

Token tokens[32];
int nr_token;


static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position]  != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i <  NR_REGEX; i ++) {
			if(regexec(& re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

		//		Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO:  Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				if(i == NOTYPE)
					continue;
				tokens[nr_token].type = i;
				strncpy(tokens[nr_token].str,substr_start,substr_len);
				nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	nr_token--;
	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int ret;
	int i = 0;
	/* TODO: Insert codes to evaluate the expression. */
//	Log("Log : eval(0,%d)",nr_token);
	for(;i <= nr_token;i++){
		if(tokens[i].type == MULT){
			if(i == 0)
				tokens[i].type = DEREF;
			else{
				int t = tokens[i - 1].type;
				if(t == PLUS || t == SUB || t == MULT || t == DIV || t == LP || t == EQ)
					tokens[i].type = DEREF;	
			}
		}
//		Log("%d : %s\n",i,tokens[i].str);
	}
	ret = eval(0,nr_token);
//	printf("%s = %d\n",e,ret);
	
//	panic("please implement me");
	return ret;
}

int eval(int p,int q){
	int disp, dtype;
	if(p > q){  
		Log("Bad expression.");
		return -1;
	} 
 	else if(p == q){
 		switch(tokens[p].type){
			case IDE :{
						  int ret = getIDEaddr(tokens[p].str);
						  if(!ret)
							  printf("No identifier named with %s\n",tokens[p].str);
						  return ret;
					  }
			case DECI : return atoi(tokens[p].str);
			case HEX : return strtoul(tokens[p].str,0,0);
			case REG32 : {
				if(strcmp(tokens[p].str,"$eax") == 0)
					return cpu.eax;
				else if(strcmp(tokens[p].str,"$ecx") == 0)
					return cpu.ecx;
				else if(strcmp(tokens[p].str,"$edx") == 0)
					return cpu.edx;
				else if(strcmp(tokens[p].str,"$ebx") == 0)
					return cpu.ebx;
				else if(strcmp(tokens[p].str,"$esp") == 0)
					return cpu.esp;
				else if(strcmp(tokens[p].str,"$ebp") == 0)
					return cpu.ebp;
				else if(strcmp(tokens[p].str,"$esi") == 0)
					return cpu.esi;
				else if(strcmp(tokens[p].str,"$edi") == 0)
					return cpu.edi;
				else{
					Log("Bad register : %s",tokens[p].str);		
					return -1;
				}
			}
			case REG16 : {
				if(strcmp(tokens[p].str,"$ax") == 0)
					return cpu.gpr[0]._16;
				else if(strcmp(tokens[p].str,"$cx") == 0)
					return cpu.gpr[1]._16;
				else if(strcmp(tokens[p].str,"$dx") == 0)
					return cpu.gpr[2]._16;
				else if(strcmp(tokens[p].str,"$bx") == 0)
					return cpu.gpr[3]._16;
				else if(strcmp(tokens[p].str,"$sp") == 0)
					return cpu.gpr[4]._16;
				else if(strcmp(tokens[p].str,"$dp") == 0)
					return cpu.gpr[5]._16;
				else if(strcmp(tokens[p].str,"$si") == 0)
					return cpu.gpr[6]._16;
				else if(strcmp(tokens[p].str,"$di") == 0)
					return cpu.gpr[7]._16;
				else{
					Log("Bad register : %s",tokens[p].str);		
					return -1;
				}
			}
			case REG8 : {
				if(strcmp(tokens[p].str,"$al") == 0)
					return cpu.gpr[0]._8[0];
				else if(strcmp(tokens[p].str,"$cl") == 0)
					return cpu.gpr[1]._8[0];
				else if(strcmp(tokens[p].str,"$dl") == 0)
					return cpu.gpr[2]._8[0];
				else if(strcmp(tokens[p].str,"$bl") == 0)
					return cpu.gpr[3]._8[0];
				else if(strcmp(tokens[p].str,"$ah") == 0)
					return cpu.gpr[4]._8[1];
				else if(strcmp(tokens[p].str,"$ch") == 0)
					return cpu.gpr[5]._8[1];
				else if(strcmp(tokens[p].str,"$dh") == 0)
					return cpu.gpr[6]._8[1];
				else if(strcmp(tokens[p].str,"$bh") == 0)
					return cpu.gpr[7]._8[1];
				else{
					Log("Bad register : %s",tokens[p].str);		
					return -1;
				}
			}

		}
	}
	else  if(p + 1 == q){
		assert(tokens[p].type == DEREF);
		swaddr_t addr = (swaddr_t)strtoul(tokens[q].str,0,0);
		return swaddr_read(addr,4);
	}
	else{ 

		if(!check_parentheses(p,q)){
			Log("Parentheses do not match.");
			return -1;
		}
		else{ 
			if(tokens[p].type == LP && tokens[q].type == RP){
				int pa = p;
				while(!check_parentheses(p,pa))
					pa++;
				if(pa == q){
			//		Log("deparent");
					return eval(p + 1,q - 1);
				}
			}
			//deparent (a+b) -> a+b
			int leq = last_token(p,q,EQ);
			assert(leq == -1 || ( leq >= p && leq <= q));
			if(leq == q || leq == p){
				Log("Syntax error : bad equal sign");	
				return -1;	
			} 
			else if(leq != -1){
				if(tokens[leq].type == EQ)
					return (eval(p,leq - 1) == eval(leq + 1,q));
				else
					return (eval(p,leq - 1) != eval(leq + 1,q));
			}
			//no equal sign
			else{
				leq = last_token(p,q,AND);
				if(leq == q || leq == p){
					Log("Syntax error : bad AND/OR sign");	
					return -1;	
				} 
				else if(leq != -1){
					if(tokens[leq].type == AND)
						return (eval(p,leq - 1) && eval(leq + 1,q));
					else
						return (eval(p,leq - 1) || eval(leq + 1,q));
				}
				else{
					if(tokens[p].type == DEREF && tokens[p + 1].type == LP && tokens[q].type){
						swaddr_t addr = (swaddr_t)eval(p + 2,q - 1);
						return swaddr_read(addr,1);
					} 
					else{
					disp = q;
					dtype = tokens[disp].type;
					while(disp >= p && dtype != PLUS && dtype != SUB){
		//				dtype = tokens[disp].type;	
						if(dtype == RP){
							int t = disp;
							while(!check_parentheses(t,disp))
								t--;
							disp = t;
							//ignore Parentheses
							if(disp == p)
								break;
							//parented
				 	 	}
						disp--;
						if(disp >= p)
							dtype = tokens[disp].type;	
				 	} 
					if(dtype == PLUS){
				//		Log("Log : eval(%d,%d) + eval(%d,%d)",p,disp - 1,disp + 1,q);
						return eval(p,disp - 1) + eval(disp + 1,q);
				 	}
					else if(dtype == SUB){
				//		Log("Log : eval(%d,%d) - eval(%d,%d)",p,disp - 1,disp + 1,q);
						return eval(p,disp - 1) - eval(disp + 1,q);
					}
						//no plus or sub or parented
					else{
						disp = q;
						dtype = tokens[disp].type;
						while(disp >= p && dtype != MULT && dtype != DIV){
							if(dtype == RP){
								int t = disp;
								while(!check_parentheses(t,disp))
									t--;
								disp = t;
				 			}	
							disp--;
							if(disp >= p)
								dtype = tokens[disp].type;
						}		
						if(dtype == MULT){
				//			Log("Log : eval(%d,%d) * eval(%d,%d)",p,disp - 1,disp + 1,q);
							return eval(p,disp - 1) * eval(disp + 1,q);
						}
						else if(dtype == DIV)
							return eval(p,disp - 1) / eval(disp + 1,q);
						else{
							Log("c");
							return eval(p + 1,q - 1);
							}
						}	
					}
			 	}
			}
		}
	}
	
	Log("Unknown error");
	return -1;
}

bool check_parentheses(int p,int q){
	int i;
	int lp = 0,rp = 0;
	for(i = p;i <= q;i++){
		if(tokens[i].type == LP)
			lp++;
		if(tokens[i].type == RP)
			rp++;
		if(lp < rp)
			return false;
	}
	if(lp == rp)
		return true;
	else
		return false;
}
int last_token(int p,int q,int type){
	assert(p < q);
	int disp,last = -1;
	switch(type){
	case EQ:{
	for(disp = p;disp <= q;disp++)
		if(tokens[disp].type == EQ || tokens[disp].type == NEQ)
			last = disp;
			break;
			}
	case AND:{
	for(disp = p;disp <= q;disp++)
		if(tokens[disp].type == AND || tokens[disp].type == OR)
			last = disp;		 
			 break;
			 }
	}
	return last;	
}
uint32_t getIDEaddr(char *ide){
	char *strtab = getStrtab();
	Log("%x",(uint32_t)strtab);
	Elf32_Sym *symtab = getSymtab();
	Log("%x",(uint32_t)symtab);
	int i = 0;
	while(symtab[i].st_info != STT_OBJECT)
		i++;
	while(symtab[i].st_info == STT_OBJECT){
		char* addr = symtab[i].st_name + strtab;
		Log("%x",(uint32_t)addr);
		if(!strcmp(ide,addr))
			return symtab[i].st_value;
		i++;
	}
	return 0;
}
