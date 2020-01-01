#include <std/stddef.h>
#include <std/stdint.h>
#include <std/string.h>

#include <lunaros/kernel.h>
#include <lunaros/page.h>
#include <lunaros/pagetable.h>
#include <lunaros/vmm.h>
#include <lunaros/x86.h>

/* VM area reserved on boot */
static pte_t *reserved;

void vmm_flush_tlb(void) {
   flush_tlb();
}
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

void *vmm_assign_map(vmm_map_t *map, void *addr, ppt_t type) {
   uint64_t vaddr = 0xFFFFll << 48;
   uint64_t pgnum = PGROUNDDOWN((uint64_t)addr, PGSIZE);
   uint16_t offset = (uint64_t)addr - pgnum;
   uint64_t base = pgnum & (0xFFFFFFFFFFll << 12); /* get base addr */
   pte_t *entry = (pte_t *)&base;
   if (entry->present)
      return NULL;
   entry->present = 1;
   entry->rw = 1;
   switch (type) {
   case PPT_UC:
      entry->pwt = 1;
      entry->pcd = 1;
      entry->pat = 1;
      break;
   case PPT_WT:
      entry->pwt = 1;
      entry->pcd = 0;
      entry->pat = 0;
      break;
   case PPT_WC: /* Not implemented */
   case PPT_WP: /* Not implemented */
   case PPT_WB: /* Default */
      /* 0's are set already */
      break;
   default:
      panic("memory type undefined");
   }
   reserved[map->pte] = *entry;
   vaddr |= (map->pml4e & 0x1FFll) << 39;
   vaddr |= (map->pdpte & 0x1FF) << 30;
   vaddr |= (map->pde & 0x1FF) << 21;
   vaddr |= (map->pte & 0x1FF) << 12;
   vaddr |= offset & 0xFFF;
   vmm_invlpg(vaddr);
   return (void *)vaddr;
}

void vmm_init(pte_t *pt) {
   reserved = pt;
}
