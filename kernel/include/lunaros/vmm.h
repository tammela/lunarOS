#pragma once

#include <lunaros/pagetable.h>

typedef struct vmm_map_t vmm_map_t;

#define VMM_MAP_INVALID (0xFFFF)

struct vmm_map_t {
   uint16_t pml4e;      /* pml4 index allocated */
   uint16_t pdpte;      /* pdpte index allocated */
   uint16_t pde;        /* pde index allocated */
   uint16_t pte;        /* pte index allocated */
};

/*
** Invalidate page on TLB cache
*/
void vmm_invlpg(uint64_t addr);

/*
** Fetch the first free map in the page table structures
*/
uint32_t vmm_fetch_map(vmm_map_t *map);

/*
** Maps the physical address `page` to `map`. Returns the virtual address.
*/
void *vmm_assign_map(vmm_map_t *map, void *page, ppt_t type);

void vmm_init(pde_t *reserved);
