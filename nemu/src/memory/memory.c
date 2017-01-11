#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_read(swaddr_t, size_t, bool *);
void cache_write(swaddr_t, size_t, uint32_t);
/* Memory accessing interfaces */

hwaddr_t page_translate(lnaddr_t addr){
	return 0;
}

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	bool success = true;
	uint32_t cache_ret = cache_read(addr,len,&success);
	uint32_t dram_ret = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	if(success){
//		if(cache_ret != dram_ret)
//			Log("R:%x %x addr:%x len:%d",cache_ret,dram_ret,addr,len);
		return cache_ret;
	}
	else
		return dram_ret;
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
	cache_write(addr, len, data);
/*	bool success = true;
	int temp = cache_read(addr, len, &success);
	if(temp != data)
		Log("W:%x %x addr:%x len:%d",temp,data,addr,len);
*/
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	if((addr >> 3) != ((addr + len) >> 3)){
		Log("data cross the page boundry!");
		assert(0);
	}
	else{
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	if((addr >> 3) != ((addr + len) >> 3)){
		Log("data cross the page boundry!");
		assert(0);
	}
	else{
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
	}
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

