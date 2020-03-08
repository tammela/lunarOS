#pragma once

#include <lunaros/buddy.h>
#include <lunaros/list.h>

#define PGSIZE (4096)
#define PGROUNDUP(sz, pgsz) (((sz) + pgsz - 1) & ~(pgsz - 1))
#define PGROUNDDOWN(sz, pgsz) (((sz)) & ~(pgsz - 1))

typedef struct {
   uint64_t base;       /* base to allocate memory from */
   uint64_t addr;       /* area initial address */
   uint64_t len;        /* total area length */
   struct page *map;    /* area's page map */
   size_t map_sz;       /* page map size */
   size_t used;         /* used memory counter in bytes */
   list_t buddies;      /* buddies in this area */
} mem_area_t;

void page_free(void *v);
void *page_alloc(size_t npages);
void page_init(mem_area_t *areas, size_t areas_sz, size_t poff);

struct page {
   struct page *next;   /* next page (used in slab) */
   void *virtual;       /* page virtual mapping */
   uint8_t order;       /* page buddy order */
};

#define pfn(addr) (addr) / PGSIZE

struct page *phys2page(void *addr);

/* physical page type */
typedef enum ppt_t ppt_t;

enum ppt_t {
   PPT_UC,      /* Strong Uncacheable */
   PPT_WC,      /* Write Combining */
   PPT_WT,      /* Write Through */
   PPT_WB,      /* Write Back (default) */
   PPT_WP       /* Write Protected */
};
