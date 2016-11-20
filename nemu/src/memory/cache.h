#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"


/*	BITS : b
 *	SIZE : B
 */
#define CACHE_LINE_BITS 6
#define CACHE_LINE_SIZE (1 << CACHE_LINE_BITS)
#define CACHE_LINE_MASK (CACHE_LINE_SIZE - 1)
#define ADDR_BITS 32
#define TAG_BITS ADDR_BITS - CACHE_LINE_BITS

#define WAY_NUM 8
#define WAY_MASK (WAY_NUM - 1)

#define SET_BITS 7
#define SET_NUM (1 << SET_BITS)
#define SET_MASK (SET_NUM - 1)

typedef	struct {
	int valid:1;
	int dirty:1;

	/* sizeof(addr) = 4B = 32bits
	 * sizeof(line) = 64bits = 2^6 bits
	 * displacement in line needs 6bits
	 * thus 26bits are demanded to specify memory addr
	 */

	uint32_t tag:TAG_BITS;
	union{
		uint8_t data[CACHE_LINE_SIZE];
		uint16_t data16[CACHE_LINE_SIZE / 2];
		uint32_t data32[CACHE_LINE_SIZE / 4];
	};
}CacheSlot;

static CacheSlot cache[SET_NUM][WAY_NUM];

void cache_init(CacheSlot *);
uint32_t cache_read(swaddr_t, size_t, bool *);
void cache_write(swaddr_t, size_t, uint32_t);
void load_cache(swaddr_t);
extern uint32_t dram_read(hwaddr_t,size_t);
/* |0000 0000 0000 0000 0000 0000 0000 0000|
 *                                 |00 0000| 	-> 64B A Slot
 *                        |0 0000 00|			-> 128 * 64B A Way
 *
 * To an address, first we match its [6,12] bits with 128 sets.
 * If hit, match its [13,31]bits with every tag of 8 ways.
*/

#endif
