#include <std/stddef.h>

#include <lunaros/mmap.h>
#include <lunaros/vmm.h>

void *mmap_physical_page(void *page, ppt_t type) {
   vmm_map_t map;
   if (vmm_fetch_map(&map))
      return NULL;
   return vmm_assign_map(&map, page, type);
}
