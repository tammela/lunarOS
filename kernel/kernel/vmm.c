#include <std/stddef.h>
#include <std/stdint.h>
#include <std/string.h>

#include <lunaros/irq.h>
#include <lunaros/kernel.h>
#include <lunaros/page.h>
#include <lunaros/pagetable.h>
#include <lunaros/vmm.h>
#include <lunaros/x86.h>

struct vm_area {
#define VM_AREA_MAX     (512)
   uint32_t size;
   pte_t *pt;
};

/* VM area reserved on boot */
struct vm_area reserved;

void *vmm_virt2phys(void *addr) {
   uintptr_t a = (uintptr_t)addr; /* for arithmetics */
   if (a >= VMM_RESERVED_BOOT_START &&
         a <= VMM_RESERVED_BOOT_END)
      return (void *)(a - VMM_RESERVED_BOOT_START);
   else if (a >= VMM_RESERVED_DM_START &&
         a <= VMM_RESERVED_DM_END)
      return (void *)(a - VMM_RESERVED_DM_START);
   else
      /* TODO */
      panic("TODO");
}

void *vmm_virt2slab(void *addr) {
   void *paddr = vmm_virt2phys(addr);
   struct page *page = phys2page(paddr);
   if (unlikely(page == NULL))
      return NULL;
   return page->slab;
}

void *vmm_phys2virt(void *addr) {
   uintptr_t a = (uintptr_t)addr;
   return (void *)(a + VMM_RESERVED_DM_START);
}

void vmm_direct_map(pml4e_t entry) {
   static uint32_t next = 273;
   pml4e_t *pml4 = vmm_pml4();
   pml4[next] = entry;
   next++;
}

uint32_t vmm_fetch_map(vmm_map_t *map, size_t npages) {
   /* TODO: implement proper virtual memory managing */
   if (!irq_enabled()) { /* use reserved page table */
      if ((reserved.size + npages) > VM_AREA_MAX)
         panic("No virtual memory mappings available, can't fallback!");
      reserved.size++;
      map->pml4e = 257;
      map->pdpte = 0;
      map->pde = 0;
      map->pte = reserved.size - 1;
      return 0;
   }
   return 1;
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
   reserved.pt[map->pte] = *entry;
   vaddr |= (map->pml4e & 0x1FFll) << 39;
   vaddr |= (map->pdpte & 0x1FF) << 30;
   vaddr |= (map->pde & 0x1FF) << 21;
   vaddr |= (map->pte & 0x1FF) << 12;
   vaddr |= offset & 0xFFF;
   vmm_invlpg(vaddr);
   return (void *)vaddr;
}

void vmm_init(pte_t *pt) {
   reserved.pt = pt;
   reserved.size = 0;
}
