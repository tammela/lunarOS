#ifndef _PAGE_H
#define _PAGE_H

#include <std/stdint.h>

#define PGSIZE         (4096)
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

struct page {
   struct page *next;   /* next free page (might not be linear) */
};

void page_init(void *vstart, void *vend);
void page_addrange(void *start, void *end);
void page_freerange(void *vstart, void *vend);
void page_free(void *v);

/*
** Page returned is not mapped!!
*/
void *page_alloc(void);

#endif /* _PAGE_H */
