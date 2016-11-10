#include "cache.h"

void cache_init(CacheSlot *this){
	this->valid = 0;
	this->dirty = 0;
	this->tag = 0;
	memset(this->data,0,CACHE_LINE_SIZE);
}

CacheSlot* cache_read(swaddr_t addr){
	int i = 0;
	int temptag = addr >> CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[temptag & SET_MASK][i];
		if(cs->valid && !cs->dirty && temptag == cs->tag)
			return cs;
	}
	return NULL;
}

void cache_write(swaddr_t addr){
	int i = 0,j = 0;
	int temptag = addr >> CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[temptag & SET_MASK][i];
		if(!cs->valid){
			cs->valid = 1;
			for(j = 0;j < CACHE_LINE_SIZE / 4;j++)
				cs->data32[i] = swaddr_read(addr,4);
		}
	}
	int random = temptag & WAY_MASK;
	CacheSlot *cs = &cache[temptag & SET_MASK][random];
	assert(cs->valid);
	for(j = 0;j < CACHE_LINE_SIZE / 4;j++)
		cs->data32[i] = swaddr_read(addr,4);
}
