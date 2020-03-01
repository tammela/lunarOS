#include <std/string.h>

#include <lunaros/mm.h>
#include <lunaros/vmm.h>
#include <lunaros/page.h>
#include <lunaros/pagetable.h>
#include <lunaros/kernel.h>

/*
** Allocate memory in a 2-page reserved area.
** Allocations in this area are not meant to be freed.
*/
void *mm_reserved_alloc(size_t sz) {
   void *ret;
   static uint64_t available = PGSIZE * 2;
   static uint8_t page[PGSIZE * 2 / sizeof(uint8_t)] = {0};
   static uint64_t base = (uint64_t)&page;
   ALIGN_TO(sz, sizeof(intmax_t));
   if (sz > available)
      return NULL;
   ret = (void *)base;
   base += sz;
   available -= sz;
   memset(ret, 0, sz);
   return ret;
}

/*
** Direct map the entire physical memory supported in 1GiB pages.
** On 64-bit we hardcode the limit to 512 GiB so far, but it's
** guaranteed to be in a reserved area of about 64TiB.
** Just like Linux...
*/
static void direct_map_everything(void) {
   uintptr_t addr;
   pml4e_t *pml4e;
   huge_pdpte_t entry = {0};
   static huge_pdpte_t pages[512] __aligned(PGSIZE) = {0};
   if (pages == NULL)
      panic("Not enough reserved memory to bootstrap memory");
   for (int i = 0; i < 1; i++) {
      for (int j = 0; j < 1; j++) {
         entry.present = true;
         entry.rw = true;
         entry.ps = true; /* 1 GiB page */
         entry.base = j + (512 * i);
         pages[j + (512 * i)] = entry;
      }
      addr = (uintptr_t)vmm_virt2phys(pages);
      pml4e = (pml4e_t *)&addr;
      pml4e->present = true;
      pml4e->rw = true;
      vmm_direct_map(*pml4e);
   }
   vmm_flush_tlb();
}

void mm_init(mem_area_t *areas, size_t areas_sz, size_t physoff, pte_t *reserved) {
   /* Direct mappings */
   direct_map_everything();
   /* Virtual memory manager */
   vmm_init(reserved);
   /* Physical page allocator */
   page_init(areas, areas_sz, physoff);
}
