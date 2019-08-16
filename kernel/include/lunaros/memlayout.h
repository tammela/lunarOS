#ifndef _MEMLAYOUT_H
#define _MEMLAYOUT_H

#include <std/stdint.h>
#include <lunaros/compiler.h>

struct mmap_entry {
	uint64_t addr;
	uint64_t length;
};

typedef void (*region_fptr_t)(uint64_t addr, uint64_t length);

void mmap_add_region(uint64_t addr, uint64_t length);

void mmap_for_each_region(region_fptr_t exec);

void mmap_print(void);

#endif /* _MEMLAYOUT_H */
