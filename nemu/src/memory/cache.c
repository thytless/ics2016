#include "cache.h"

void cache_init(CacheSlot *this){
	this->valid = 0;
	this->dirty = 0;
	this->tag = 0;
	memset(this->data,0,CACHE_LINE_SIZE);
}

uint32_t cache_read(swaddr_t addr, size_t size, bool *success){
	int i = 0;
	int temptag = addr >> CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[temptag & SET_MASK][i];
		if(cs->valid && !cs->dirty && temptag == cs->tag){
			uint32_t disp = addr & CACHE_LINE_MASK;
			if(disp + size > CACHE_LINE_SIZE){
				int lsize = CACHE_LINE_SIZE - disp - 1;
				int rsize = size - lsize;
				assert(rsize);
				int ldata = 0;
				int rdata = 0;
				if(lsize)
					ldata = cache_read(addr,lsize,success);
				if(*success){
					rdata = cache_read(addr + lsize,rsize,success);
					if(*success)
						return (ldata << (lsize * 8)) + rdata;
				}
				return 0;
			}
			else{
				*success = true;
				switch(size){
					case 1: return cs->data[disp];
					case 2: return cs->data16[disp / 2];
					case 4: return cs->data32[disp / 4];
					default:panic("Bad Size!");
				}
			}
		}
	}
	*success = false;
	load_cache(temptag);
	return 0;
}

void cache_write(swaddr_t addr, size_t size, uint32_t data){
	int i = 0;
	int temptag = addr >> CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[temptag & SET_MASK][i];
		if(cs->valid && !cs->dirty && temptag == cs->tag){
			uint32_t disp = addr & CACHE_LINE_MASK;
			if(disp + size > CACHE_LINE_SIZE){
				int lsize = CACHE_LINE_SIZE - disp - 1;
				int rsize = size - lsize;
				int mask = (1 << rsize * 8) - 1;
				assert(rsize);
				if(lsize)
					cache_write(addr,lsize,data >> (rsize * 8));
				cache_write(addr+lsize,rsize,data & mask);
				return;
			}
			else{
				switch(size){
					case 1: cs->data[disp] = data & 0xff;break;
					case 2: cs->data16[disp / 2] = data & 0xffff;break;
					case 4: cs->data32[disp / 4] = data;break;
					default:panic("Bad Size!");break;
				}
				return;
			}
		}
	}
	load_cache(temptag);
	return;
}

void load_cache(swaddr_t temptag){
	int i = 0,j = 0;
	uint32_t start = temptag << CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[temptag & SET_MASK][i];
		if(!cs->valid){
			cs->valid = 1;
			cs->tag = temptag;
			for(j = 0;j < CACHE_LINE_SIZE / 4;j++)
				cs->data32[j] = dram_read(start + 4 * j,4);
			return;
		}
	}
	int random = temptag & WAY_MASK;
	CacheSlot *cs = &cache[temptag & SET_MASK][random];
	assert(cs->valid);
	cs->tag = temptag;
	for(j = 0;j < CACHE_LINE_SIZE / 4;j++)
		cs->data32[j] = dram_read(start + 4 * j,4);
	return;
}
