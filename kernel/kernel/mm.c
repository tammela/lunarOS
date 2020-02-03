#include <lunaros/mm.h>
#include <lunaros/vmm.h>
#include <lunaros/page.h>

/*
** Allocate memory in a 2-page reserved area.
** Allocations in this area are not meant to be freed.
*/
void *mm_reserved_alloc(size_t sz) {
   static uint64_t available = PGSIZE * 2;
   static uint8_t page[PGSIZE * 2 / sizeof(uint8_t)] = {0};
   static uint64_t base = (uint64_t)&page;
   if (sz > available)
      return NULL;
   void *ret = (void *)base;
   base += sz;
   available -= sz;
   return ret;
}

void mm_init(physmem_layout_t **layouts, size_t physoff, pte_t *reserved) {
   /* Virtual memory manager */
   vmm_init(reserved);
   /* Physical page allocator */
   page_init(layouts, physoff);
}
