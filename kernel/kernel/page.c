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
** Reserved memory for ELF
*/
static size_t physoff;

/*
** That's how we see the available physical memory in the system
*/
mem_area_t *phys_mem = NULL;
size_t phys_mem_sz = 0;

struct page *phys2page(void *addr) {
   uintptr_t address = (uintptr_t)addr;
   if (!address)
      goto bad;
   for (size_t i = 0; i < phys_mem_sz; i++) {
      size_t map_index = 0;
      if (phys_mem[i].addr + phys_mem[i].len < address)
         continue;
      map_index = pfn(address) - pfn(phys_mem[i].addr);
      return &phys_mem[i].map[map_index];
   }
bad:
   return NULL;
}

void *page2phys(struct page *page) {
   uintptr_t address = (uintptr_t)page;
   if (!address)
      goto bad;
   for (size_t i = 0; i < phys_mem_sz; i++) {
      size_t offset = 0;
      if (phys_mem[i].addr + phys_mem[i].len < address)
         continue;
      offset = (uintptr_t)page - (uintptr_t)phys_mem[i].map;
      return (void *)((PGSIZE * offset / sizeof(struct page))
               + phys_mem[i].addr);
   }
bad:
   return NULL;
}

void *page_alloc(size_t npages) {
   void *p;
   list_t *cursor;
   for (size_t i = 0; i < phys_mem_sz; i++) {
      list_for_each (cursor, &phys_mem[i].buddies) {
         buddy_area_t *b = container_of(cursor, buddy_area_t, head);
         if (b->available < PGSIZE * npages)
            continue;
         p = buddy_alloc(b, npages);
         if (p) {
            struct page *page = phys2page(p);
            if (page == NULL)
               panic("Address returned by buddy has no struct page\n");
            page->buddy = b;
         }
         return p;
      }
   }
   return NULL;
}

void page_free(void *addr) {
   struct page *page;
   if (addr == NULL)
      return;
   page = phys2page(addr);
   if (page == NULL)
      panic("Address returned by buddy has no struct page\n");
   buddy_free(page->buddy, addr);
}

#define BUDDY_MAX_SIZE  (1 << 19)   /* 512K */

static void bootstrap_page_allocator(void) {
   for (size_t i = 0; i < phys_mem_sz; i++) {
      buddy_area_t *b;
      if (phys_mem[i].len < BUDDY_MAX_SIZE)
         continue;
      b = buddy_area_init((void *)phys_mem[i].base,
            BUDDY_MAX_SIZE, mm_reserved_alloc);
      if (unlikely(b == NULL))
         panic("Not enough reserved memory to bootstrap the page allocator!\n");
      list_init(&phys_mem[i].buddies);
      list_pushback(&phys_mem[i].buddies, &b->head);
   }
   pr_debug("Page Allocator initialized\n");
}

static void init_mem_map() {
   for (size_t i = 0; i < phys_mem_sz; i++) {
      size_t npages = (phys_mem[i].len - phys_mem[i].used) / PGSIZE;
      size_t alloc_sz = npages * sizeof(struct page);
      if (unlikely(phys_mem[i].len < alloc_sz))
         panic("Cannot allocate memory map\n");
      phys_mem[i].map_sz = npages;
      phys_mem[i].map = vmm_phys2virt((void *)phys_mem[i].addr);
      /* reserve area on layout */
      phys_mem[i].base = (uintptr_t)phys_mem[i].addr + alloc_sz;
      phys_mem[i].used += alloc_sz;
      ALIGN_TO(phys_mem[i].base, sizeof(uintmax_t));
      for (size_t j = 0; j < npages; j++)
         memset(&phys_mem[i].map[j], 0, sizeof(struct page));
      pr_debug("Reserved area of %d bytes for memory map\n", alloc_sz);
   }
}

void page_init(mem_area_t *areas, size_t sz, size_t poff) {
   size_t npages = 0;
   uint64_t available = 0;
   physoff = poff;
   phys_mem = areas;
   phys_mem_sz = sz;
   for (size_t i = 0; i < phys_mem_sz; i++) {
      uint64_t minaddr = PGROUNDUP(phys_mem[i].addr, PGSIZE);
      if (minaddr == 0) { /* beginning of physical memory */
         phys_mem[i].addr = PGROUNDUP(poff, PGSIZE);
         phys_mem[i].used = PGROUNDUP(poff, PGSIZE);
      }
      available += phys_mem[i].len;
   }
   npages = available / PGSIZE;
   pr_debug("%d KiB available in physical memory\n", available / (1 << 10));
   pr_debug("%d pages available\n", npages);
   init_mem_map();
   bootstrap_page_allocator();
}
