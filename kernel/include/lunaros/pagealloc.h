#ifndef _PAGEALLOC_H
#define _PAGEALLOC_H

#define PGSIZE         4096
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

void pagealloc_init(void *vstart, void *vend);

void pagealloc_addrange(unsigned long saddr, unsigned long eaddr);

void pagealloc_freerange(void *vstart, void *vend);

void pagealloc_free(char *v);

char *pagealloc_alloc(void);

#endif /* _PAGEALLOC_H */
