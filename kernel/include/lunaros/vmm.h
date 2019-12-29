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

uint32_t vmm_fetch_map(vmm_map_t *map);
void *vmm_assign_map(vmm_map_t *map, void *page, ppt_t type);
void vmm_init(pde_t *reserved);
