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
static LIST_HEAD(buddies);

/*
** That's how we see the available physical memory in the system
*/
static physmem_layout_t physmem[MM_PHYSMEM_LAYOUT_SZ];

void *page_alloc(size_t npages) {
   list_t *cursor;
   list_for_each (cursor, &buddies) {
      buddy_area_t *b = container_of(cursor, buddy_area_t, head);
      if (b->available >= PGSIZE * npages)
         return buddy_alloc(b, PGSIZE * npages);
   }
   return NULL;
}

void page_free(void *page) {
   list_t *cursor;
   uintptr_t addr = (uintptr_t)page;
   list_for_each (cursor, &buddies) {
      buddy_area_t *b = container_of(cursor, buddy_area_t, head);
      uintptr_t base = (uintptr_t)b->base;
      if (addr >= (base + b->min) && addr <= (base + b->max)) {
         buddy_free(b, page);
         break;
      }
   }
}

#define BUDDY_MAX_SIZE  (1 << 20)   /* 1 MiB */

static void bootstrap_page_allocator(void) {
   for (size_t i = 0; i < MM_PHYSMEM_LAYOUT_SZ; i++) {
      buddy_area_t *b;
      if (physmem[i].addr == 0)
         continue;
      if (physmem[i].len < BUDDY_MAX_SIZE)
         continue;
      b = buddy_area_init(vmm_phys2virt((void *)physmem[i].addr),
            BUDDY_MAX_SIZE, mm_reserved_alloc);
      if (unlikely(b == NULL))
         panic("Not enough reserved memory to bootstrap the page allocator!\n");
      list_pushback(&buddies, &b->head);
      break;
   }
   pr_debug("Page Allocator initialized\n");
}

void page_init(physmem_layout_t **layouts, size_t poff) {
   uint64_t available = 0;
   physoff = poff;
   for (size_t i = 0; i < MM_PHYSMEM_LAYOUT_SZ; i++) {
      uint64_t maxaddr, minaddr;
      if (layouts[i] == NULL)
         continue;
      minaddr = PGROUNDUP(layouts[i]->addr, PGSIZE);
      maxaddr = PGROUNDDOWN(layouts[i]->addr + layouts[i]->len, PGSIZE);
      physmem[i].addr = minaddr;
      if (minaddr == 0) /* beginning of physical memory */
         physmem[i].addr = PGROUNDUP(poff, PGSIZE);
      physmem[i].len = maxaddr - minaddr;
      available += physmem[i].len;
   }
   pr_debug("%d KiB available in physical memory\n", available / (1 << 10));
   /* Bootstrap our page frame allocator */
   bootstrap_page_allocator();
}
