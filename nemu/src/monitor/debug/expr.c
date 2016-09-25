#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum { 
	LP = '(', RP = ')',
	NOTYPE = 256, PLUS = '+', SUB = '-', MULT = '*', DIV = '/',
	DEREF, EQ, HEX, DECI, IDE, REG32, REG16, REG8

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
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"^(0[xX])?[0-9a-fA-F]+", HEX},	// hex
	{"[0-9]+", DECI},				// decimal
	{"-", '-'},						// substract
	{"\\*", '*'},					// multiply
	{"/", '/'},						// divide
	{"\\(", '('},					// left bracket
	{"\\)", ')'},					// right bracket
	{"\\$e(?:ax|bx|cx|dx|di|si|bp|sp)", REG32},
									// 32-bit register
	{"\\$(?:bx|cx|ax|dx|di|si|bp|sp)", REG16},
	{"\\$(?:ah|bh|ch|dh|al|bl|cl|dl)", REG8},
//	{"(?<=\\+|-|\\*|/|\\()\\*", DEREF},	// dereference
	{"[_a-zA-Z][_0-9a-zA-Z]*", IDE}		// identifier	

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];
int eval(int,int);
bool check_parentheses(int,int);
int last_eq(int,int);
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
	int i,last;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				tokens[nr_token].type = i;
				if(i == MULT)
					if(last == PLUS || last == SUB || last == MULT || last == DIV || last == LP || last == EQ)
						i = DEREF;
				if(strcpy(tokens[nr_token].str,substr_start))
					nr_token++;
/*				switch(rules[i].token_type) {
					case NOTYPE : 
					case PLUS : 
					case SUB :
					case MULT :
					case DIV : 
					case DEREF :
					case EQ : 
					case HEX_S : 
					case HEX :
					case DECI :
					case IDE :
					case LP :
					case RP :
					case REG32 : 
					case REG16 :
					case REG8 : 
					default: panic("please implement me");
				}
*/				
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int ret;
	/* TODO: Insert codes to evaluate the expression. */
	ret = eval(0,nr_token);
	printf("%s = %d\n",e,ret);
	
//	panic("please implement me");
	return 0;
}

int eval(int p,int q){
	int disp, dtype;
	if(p > q)
		panic("Bad expression.");
	else if(p == q){
		switch(tokens[p].type){
			case IDE : return 0;
			case DECI : return atoi(tokens[p].str);
			case HEX : return strtoul(tokens[p].str,0,0);
			case REG32 : {
				if(strcmp(tokens[p].str,"$eax") == 0)
					return cpu.eax;
				if(strcmp(tokens[p].str,"$ecx") == 0)
					return cpu.ecx;
				if(strcmp(tokens[p].str,"$edx") == 0)
					return cpu.edx;
				if(strcmp(tokens[p].str,"$ebx") == 0)
					return cpu.ebx;
				if(strcmp(tokens[p].str,"$esp") == 0)
					return cpu.esp;
				if(strcmp(tokens[p].str,"$ebp") == 0)
					return cpu.ebp;
				if(strcmp(tokens[p].str,"$esi") == 0)
					return cpu.esi;
				if(strcmp(tokens[p].str,"$edi") == 0)
					return cpu.edi;
			}
			case REG16 : {
				if(strcmp(tokens[p].str,"$ax") == 0)
					return cpu.gpr[0]._16;
				if(strcmp(tokens[p].str,"$cx") == 0)
					return cpu.gpr[1]._16;
				if(strcmp(tokens[p].str,"$dx") == 0)
					return cpu.gpr[2]._16;
				if(strcmp(tokens[p].str,"$bx") == 0)
					return cpu.gpr[3]._16;
				if(strcmp(tokens[p].str,"$sp") == 0)
					return cpu.gpr[4]._16;
				if(strcmp(tokens[p].str,"$dp") == 0)
					return cpu.gpr[5]._16;
				if(strcmp(tokens[p].str,"$si") == 0)
					return cpu.gpr[6]._16;
				if(strcmp(tokens[p].str,"$di") == 0)
					return cpu.gpr[7]._16;
			}
			case REG8 : {
				if(strcmp(tokens[p].str,"$al") == 0)
					return cpu.gpr[0]._8[0];
				if(strcmp(tokens[p].str,"$cl") == 0)
					return cpu.gpr[1]._8[0];
				if(strcmp(tokens[p].str,"$dl") == 0)
					return cpu.gpr[2]._8[0];
				if(strcmp(tokens[p].str,"$bl") == 0)
					return cpu.gpr[3]._8[0];
				if(strcmp(tokens[p].str,"$ah") == 0)
					return cpu.gpr[4]._8[1];
				if(strcmp(tokens[p].str,"$ch") == 0)
					return cpu.gpr[5]._8[1];
				if(strcmp(tokens[p].str,"$dh") == 0)
					return cpu.gpr[6]._8[1];
				if(strcmp(tokens[p].str,"$bh") == 0)
					return cpu.gpr[7]._8[1];
			}
		}	 
	}
	else if(p + 1 == q){
		assert(tokens[p].type == DEREF);
		swaddr_t addr = (swaddr_t)strtoul(tokens[q].str,0,0);
		return swaddr_read(addr,1);
	}
	else{
		if(!check_parentheses(p,q))
			panic("Parentheses do not match.");
		else{
			int leq = last_eq(p,q);
			assert(leq == -1 || ( leq >= p && leq <= q));
			if(leq == q || leq == p)
				panic("Syntax error : bad equal sign");	
			else if(leq != -1)
				return (eval(p,leq - 1) == eval(leq + 1,q) ? 1 : 0);
			//no equal sign
			else{
				disp = p;
				dtype = tokens[disp].type;
				while(disp <= q && dtype != PLUS && dtype != SUB){
					if(dtype == LP){
						int t = disp;
						while(!check_parentheses(disp,t))
							t++;
						disp = t;
						if(disp == q)
							break;
					}
					disp++;
					dtype = tokens[disp].type;	
				}
				if(dtype == PLUS)
					return eval(p,disp - 1) + eval(disp + 1,q);
				else if(dtype == SUB)
					return eval(p,disp - 1) - eval(disp + 1,q);
				//no plus or sub
				else{
					disp = p;
					dtype = tokens[disp].type;
					if(dtype == LP){
						int t = disp;
						while(!check_parentheses(disp,t))
							t++;
						disp = t;
					}
					while(disp <= q && dtype != MULT && dtype != DIV){
						disp++;
						dtype = tokens[disp].type;
					}
					if(dtype == MULT)
						return eval(p,disp - 1) * eval(disp + 1,q);
					else if(dtype == DIV)
						return eval(p,disp - 1) / eval(disp + 1,q);
					else
						return eval(p + 1,q - 1);
				}
			}
		}
/*	 	else if(tokens[p].type == LP){
			int disp = p;
			while(disp <= q && tokens[disp].type != RP)
				disp++;
			switch(tokens[disp + 1].type){
				case PLUS : return eval(p + 1,disp - 1) + eval(disp + 2,q);
				case SUB : return eval(p + 1,disp - 1) + eval(disp + 2,q);
				case DIV :
				case MULT : {
					tokens[disp].type = DECI;
					strcmp(tokens[disp].str,itoa(eval(p + 1,disp - 1)));
					return eval(disp,q);
				} 
			}
		}
		else{
			int domi = p;
			while(domi <= q && tokens[domi].type != PLUS && tokens[domi].type != SUB)
				domi++;
			if(domi == p){
				domi = p;
				while(domi <= q && tokens[domi].type != MULT && tokens[domi].type != DIV)
					domi++;
				if(domi == q){
					assert(p + 1 == q && tokens[p].type == DEREF);
					uint32_t *addr = (uint32_t *)strtoul(tokens[q].str,0,0);
					return swaddr_read(addr);
				}
			}
		}
		*/
	}
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
int last_eq(int p,int q){
	assert(p < q);
	int disp,last = -1;
	for(disp = p;disp <= q;disp++)
		if(tokens[disp].type == EQ)
			last = disp;
	return last;	
}
