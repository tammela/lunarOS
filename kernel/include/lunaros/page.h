#pragma once

#define PGSIZE (4096)
#define PGROUNDUP(sz, pgsz) (((sz) + pgsz - 1) & ~(pgsz - 1))
#define PGROUNDDOWN(sz, pgsz) (((sz)) & ~(pgsz - 1))

typedef struct physmem_layout_t physmem_layout_t;

struct physmem_layout_t {
   uint64_t addr;
   uint64_t len;
};

void page_free(void *v, size_t npages);
void *page_alloc(size_t npages);
void page_init(physmem_layout_t **layouts, size_t poff);

/* physical page type */
typedef enum ppt_t ppt_t;

enum ppt_t {
   PPT_UC,      /* Strong Uncacheable */
   PPT_WC,      /* Write Combining */
   PPT_WT,      /* Write Through */
   PPT_WB,      /* Write Back (default) */
   PPT_WP       /* Write Protected */
};
