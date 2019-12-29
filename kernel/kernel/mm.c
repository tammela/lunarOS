#include <lunaros/mm.h>
#include <lunaros/vmm.h>
#include <lunaros/page.h>

void mm_init(physmem_layout_t **layouts, size_t physoff, pde_t *reserved) {
   /* Virtual memory manager */
   vmm_init(reserved);
   /* Physical page allocator */
   page_init(layouts, physoff);
}
