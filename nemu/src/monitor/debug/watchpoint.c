#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	WP *p = free_;
	WP *pre = p;
	WP *nwp = free_;
	if(p == NULL){
		printf("No vacant watchpoint\n");
		return NULL;
	}
	free_ = free_ -> next;
	p = head;
	while(p != NULL && nwp -> NO > p -> NO){
		pre = p;
		p = p -> next;
	}
	if(p == head){
		nwp -> next = head;
		head = nwp;
	}
	else{
		nwp -> next = p;
		pre -> next = nwp;
	}
	return nwp;
}

void free_wp(WP *wp){
	WP *p = head;
	WP *pre = p;
	if(p == NULL){
		printf("Invalid watchpoint\n");
		return;
	}
	while(pre != NULL && pre -> next != wp)
		pre = pre -> next;
	if(pre == NULL){
		printf("watchpoint %d is not working\n",wp -> NO);
		return;
	}
	pre -> next = wp -> next;
	wp -> next = NULL;
	
	p = free_;
	while(p != NULL && wp -> NO > p -> NO){
		pre = p;
		p = p -> next;
	}
	if(p == free_){
		wp -> next = free_;
		free_ = wp;
	}
	else{
		wp -> next = p;
		pre -> next = wp;
	}
	return;
}

WP* get_head(){
	return head;
}

WP*	get_pool(){
	return wp_pool;
}
