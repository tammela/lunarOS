#include <std/stddef.h>
#include <std/stdint.h>
#include <std/string.h>

#include <lunaros/kernel.h>
#include <lunaros/page.h>
#include <lunaros/pagetable.h>
#include <lunaros/vmm.h>
#include <lunaros/x86.h>

/* VM area reserved on boot, scrapped tables */
static pde_t *reserved;

void vmm_invlpg(uint64_t addr) {
   invlpg(addr);
}

uint32_t vmm_fetch_map(vmm_map_t *map) {
   /* TODO: implement proper virtual memory managing */
   map->pml4e = 257;
   map->pdpte = 0;
   map->pde = 0;
   map->pte = 0;
   return 0;
}

void *vmm_assign_map(vmm_map_t *map, void *page, ppt_t type) {
   uint64_t vaddr = 0xFFFFll << 48;
   uint64_t base = PGROUNDDOWN((uint64_t)page, PGSIZE);
   uint16_t offset = (uint64_t)page - base;
   if (reserved[map->pte].present)
      return NULL;
   memset(&reserved[map->pte], 0, sizeof(pde_t));
   reserved[map->pte].present = 1;
   reserved[map->pte].rw = 1;
   reserved[map->pte].base = base;
   switch (type) {
   case PPT_UC:
      reserved[map->pte].pwt = 1;
      reserved[map->pte].pcd = 1;
      reserved[map->pte].pat = 1;
      break;
   case PPT_WT:
      reserved[map->pte].pwt = 1;
      reserved[map->pte].pcd = 0;
      reserved[map->pte].pat = 0;
      break;
   case PPT_WC: /* Not implemented */
   case PPT_WP: /* Not implemented */
   case PPT_WB: /* Default */
      /* 0's are set already */
      break;
   default:
      panic("memory type undefined");
   }
   vaddr |= (map->pml4e & 0x1FFll) << 39;
   vaddr |= (map->pdpte & 0x1FF) << 30;
   vaddr |= (map->pde & 0x1FF) << 21;
   vaddr |= (map->pte & 0x1FF) << 12;
   vaddr |= offset & 0xFFF;
   vmm_invlpg(vaddr);
   return (void *)vaddr;
}

void vmm_init(pde_t *pt) {
   reserved = pt;
}
