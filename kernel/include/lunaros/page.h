#pragma once

#define PGSIZE (4096)
#define PGROUNDUP(sz) (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE - 1))

struct page {
   struct page *next; /* next free page (might not be linear) */
};

typedef struct physmem_layout_t physmem_layout_t;

struct physmem_layout_t {
   uint64_t addr;
   uint64_t len;
};

void page_init(void *vstart, void *vend);
void page_addrange(void *start, void *end);
void page_freerange(void *vstart, void *vend);
void page_free(void *v);

/*
** Page returned is not mapped!!
*/
void *page_alloc(void);
