#include <std/stddef.h>

#include <lunaros/mmap.h>
#include <lunaros/vmm.h>

void *mmap_physical_page(void *page, ppt_t type) {
   vmm_map_t map;
   if (vmm_fetch_map(&map, 1))
      return NULL;
   return vmm_assign_map(&map, page, type);
}

void *mmap_physical_area(void *start, size_t npages, ppt_t type) {
   vmm_map_t map;
   void *ret;
   if (npages == 1)
      return mmap_physical_page(start, type);
   if (vmm_fetch_map(&map, npages))
      return NULL;
   ret = vmm_assign_map(&map, start, type);
   for (size_t n = 1; n < npages; n++) {
      uint64_t next = (uint64_t)start + PGSIZE * n;
      map.pte += 1;
      vmm_assign_map(&map, (void *)next, type);
   }
   return ret;
}
