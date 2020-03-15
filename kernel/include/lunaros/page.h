#pragma once

#include <lunaros/buddy.h>
#include <lunaros/list.h>
#include <lunaros/slab.h>

#define PGSIZE (4096)
#define PGROUNDUP(sz, pgsz) (((sz) + pgsz - 1) & ~(pgsz - 1))
#define PGROUNDDOWN(sz, pgsz) (((sz)) & ~(pgsz - 1))

typedef struct {
   uint64_t addr;       /* area initial address */
   uint64_t len;        /* total area length */
   uint64_t base;       /* base to allocate memory from */
   struct page *map;    /* area's page map */
   size_t map_sz;       /* page map size */
   size_t used;         /* used memory counter in bytes */
   list_t buddies;      /* buddies in this area */
} mem_area_t;

void page_free(void *v);
void *page_alloc(size_t npages);
void page_init(mem_area_t *areas, size_t areas_sz, size_t poff);

/* page status type */
typedef enum ps_t ps_t;

enum ps_t {
   PAGE_INACTIVE,    /* page is not used */
   PAGE_ACTIVE_KERN, /* page mapped to kernel space */
   PAGE_ACTIVE_USER, /* page mapped to user space */
};

struct page {
   struct kmem_cache_node *slab;   /* slab cache that holds this page */
   buddy_area_t *buddy;             /* buddy that manages this page */
   uint8_t order;                   /* page buddy order */
   ps_t status;                     /* page type */
};

#define pfn(addr) ((addr) / PGSIZE)

struct page *phys2page(void *addr);
void *page2phys(struct page *page);

/* physical page type */
typedef enum ppt_t ppt_t;

enum ppt_t {
   PPT_UC,      /* Strong Uncacheable */
   PPT_WC,      /* Write Combining */
   PPT_WT,      /* Write Through */
   PPT_WB,      /* Write Back (default) */
   PPT_WP       /* Write Protected */
};
