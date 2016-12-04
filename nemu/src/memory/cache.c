#include "cache.h"

void cache_init(CacheSlot *this){
	this->valid = 0;
	this->tag = 0;
	memset(this->data,0,CACHE_LINE_SIZE);
}

uint32_t cache_read(swaddr_t addr, size_t size, bool *success){
	int i = 0;
	int longtag = addr >> CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[longtag & SET_MASK][i];
		if(cs->valid && (longtag >> SET_BITS) == cs->tag){
			uint32_t disp = addr & CACHE_LINE_MASK;
			if(disp + size > CACHE_LINE_SIZE){
				int lsize = CACHE_LINE_SIZE - disp;
				int rsize = size - lsize;
				assert(lsize > 0 && rsize > 0);
				int ldata = 0;
				int rdata = 0;
				ldata = cache_read(addr,lsize,success);
				if(*success){
					rdata = cache_read(addr + lsize,rsize,success);
					if(*success)
						return (rdata << (lsize * 8)) + ldata;
				}
				return -1;

			}
			else{
				*success = true;
				switch(size){
					case 1: return unalign_rw((uint8_t *)cs->data + disp,1);
					case 2: return unalign_rw((uint8_t *)cs->data + disp,2);
					case 4: return unalign_rw((uint8_t *)cs->data + disp,4);
					default:panic("Bad Size!");
				}
			}
		}
	}
	*success = false;
	load_cache(longtag);
	return -1;
}

void cache_write(swaddr_t addr, size_t size, uint32_t data){
	int i = 0;
	int longtag = addr >> CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[longtag & SET_MASK][i];
		if(cs->valid && (longtag >> SET_BITS) == cs->tag){
			uint32_t disp = addr & CACHE_LINE_MASK;
			if(disp + size > CACHE_LINE_SIZE){
				int lsize = CACHE_LINE_SIZE - disp;
				int rsize = size - lsize;
				int mask = (1 << rsize * 8) - 1;
				assert(rsize);
				cache_write(addr,lsize,data & mask);
				cache_write(addr+lsize,rsize,data >> (rsize * 8));
				return;
			}
			else{
				switch(size){
					case 1: unalign_rw((uint8_t *)cs->data + disp,1) = data & 0xff;break;
					case 2: unalign_rw((uint8_t *)cs->data + disp,2) = data & 0xffff;break;
					case 3: unalign_rw((uint8_t *)cs->data + disp,3) = data & 0xffffff;break;
					case 4: unalign_rw((uint8_t *)cs->data + disp,4) = data;break;
					default:panic("Bad Size!");break;
				}
				return;
			}
		}
	}
	load_cache(longtag);
	return;
}

void load_cache(swaddr_t longtag){
	int i = 0,j = 0;
	uint32_t start = longtag << CACHE_LINE_BITS;
	for(;i < WAY_NUM;i++){
		CacheSlot *cs = &cache[longtag & SET_MASK][i];
		if(!cs->valid){
			cs->valid = 1;
			cs->tag = longtag >> SET_BITS;
			for(j = 0;j < CACHE_LINE_SIZE / 4;j++)
				cs->data[j] = dram_read(start + j * 4,4);
			return;
		}
	}
	int random = longtag & WAY_MASK;
	CacheSlot *cs = &cache[longtag & SET_MASK][random];
	assert(cs->valid);
	cs->tag = longtag >> SET_BITS;
	for(j = 0;j < CACHE_LINE_SIZE / 4;j++)
		cs->data[j] = dram_read(start + j * 4,4);
	return;
}
