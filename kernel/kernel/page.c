#include <std/stdbool.h>
#include <std/stddef.h>
#include <std/stdint.h>
#include <std/string.h>

#include <lunaros/buddy.h>
#include <lunaros/kernel.h>
#include <lunaros/mm.h>
#include <lunaros/mmap.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>
#include <lunaros/vmm.h>

/*
** Reserved memory for elf
*/
static size_t physoff;

/*
** Binary buddies used for page frame allocation
*/
static list_t buddies;

/*
** That's how we see the available physical memory in the system
*/
static physmem_layout_t physmem[MM_PHYSMEM_LAYOUT_SZ];

/*
** Direct map the entire physical memory supported.
** On 64-bit we hardcode the limit to 512 GiB so far, but it's
** guaranteed to be in a reserved an area of about 64TiB.
** Just like Linux...
*/
static void direct_map_everything(void) {
   huge_pdpte_t entry = {0};
   pml4e_t pml4e;
   huge_pdpte_t *pages = mm_reserved_alloc(512 * sizeof(huge_pdpte_t));
   if (pages == NULL)
      panic("Not enough memory to bootstrap memory");
   for (int i = 0; i < 1; i++) {
      for (int j = 0; j < 512; j++) {
         entry.present = true;
         entry.rw = true;
         entry.ps = true; /* 1 GiB page */
         entry.base = j + (512 * i);
         pages[j + (512 * i)] = entry;
      }
      uint64_t p = (uint64_t)vmm_virt2phys(pages);
      memcpy(&pml4e, &p, sizeof(p));
      pml4e.present = true;
      pml4e.rw = true;
      vmm_direct_map(pml4e);
   }
   vmm_flush_tlb();
}

/* Open up the first buddy with 512K */
#define _BUDDY_MAX      (1 << 19)

static void bootstrap_page_allocator(void *base) {
   buddy_area_t *b = buddy_area_init(base, _BUDDY_MAX, mm_reserved_alloc);
   list_init(&buddies);
   list_pushback(&buddies, &b->head);
   pr_debug("Page Allocator initialized\n");
}

void page_init(physmem_layout_t **layouts, size_t poff) {
   void *base;
   uint64_t available = 0;
   int32_t candidate = -1;
   physoff = poff;
   for (int i = 0; i < MM_PHYSMEM_LAYOUT_SZ; i++) {
      uint64_t minaddr;
      uint64_t maxaddr;
      if (layouts[i] == NULL)
         continue;
      minaddr = PGROUNDUP(layouts[i]->addr, PGSIZE);
      maxaddr = PGROUNDDOWN(layouts[i]->addr + layouts[i]->len, PGSIZE);
      physmem[i].addr = minaddr;
      if (minaddr == 0) /* beginning of physical memory */
         physmem[i].addr = PGROUNDUP(poff, PGSIZE);
      physmem[i].len = maxaddr - minaddr;
      available += physmem[i].len;
      if (candidate == -1 && (int64_t)(physmem[i].len - _BUDDY_MAX) >= 0)
         candidate = i;
   }
   pr_debug("%d KiB available in physical memory\n", available / (1 << 10));
   /* Direct map the whole physical memory */
   direct_map_everything();
   /* Bootstrap our page frame allocator */
   base = vmm_phys2virt((void *)physmem[candidate].addr);
   bootstrap_page_allocator(base);
}
