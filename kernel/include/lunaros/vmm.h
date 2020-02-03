#pragma once

#include <lunaros/pagetable.h>
#include <lunaros/x86.h>

typedef struct vmm_map_t vmm_map_t;

#define VMM_KERNEL_START        (0xffff800000000000)

/* Known reserved areas in the virtual address space */
#define VMM_RESERVED_BOOT_START (VMM_KERNEL_START)
#define VMM_RESERVED_BOOT_END   (0xffff807fffffffff)
#define VMM_RESERVED_DM_START   (0xffff888000000000)
#define VMM_RESERVED_DM_END     (0xffffc87fffffffff)

#define VMM_MAP_INVALID         (0xffff)

struct vmm_map_t {
   uint16_t pml4e;      /* pml4 index allocated */
   uint16_t pdpte;      /* pdpte index allocated */
   uint16_t pde;        /* pde index allocated */
   uint16_t pte;        /* pte index allocated */
};

void *vmm_phys2virt(void *addr);
void *vmm_virt2phys(void *addr);

/*
** Flush the entire TLB cache. Global pages are ignored.
*/
static __inline void vmm_flush_tlb(void) {
   flush_tlb();
}

/*
** Invalidate page on TLB cache
*/
static __inline void vmm_invlpg(uint64_t addr) {
   invlpg(addr);
}

/* Returns the virtual address of the pml4 */
static __inline pml4e_t *vmm_pml4(void) {
   return (pml4e_t *)(pml4() + VMM_KERNEL_START);
}

/*
** Fetch the first free map in the page table structures
*/
uint32_t vmm_fetch_map(vmm_map_t *map, size_t npages);

/*
** Maps the physical address `page` to `map`. Returns the virtual address.
*/
void *vmm_assign_map(vmm_map_t *map, void *page, ppt_t type);

/*
** Maps the PML-4 entry to the kernel's direct mappings area
*/
void vmm_direct_map(pml4e_t entry);

void vmm_init(pte_t *reserved);
