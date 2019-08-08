#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/multiboot.h>
#include <lunaros/pagealloc.h>
#include <lunaros/printf.h>

void pagealloc_freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
   struct run *next;
};

struct {
   struct run *freelist;
} kmem;

void pagealloc_init(void *vstart, void *vend) {
   pagealloc_freerange(vstart, vend);
}

void pagealloc_addrange(unsigned long saddr, unsigned long eaddr) {
   pagealloc_freerange((void *)saddr, (void *)eaddr);
}

void pagealloc_freerange(void *vstart, void *vend) {
   char *p;
   p = (char *)PGROUNDUP((uintptr_t)vstart);
   for (; p + PGSIZE <= (char *)vend; p += PGSIZE) pagealloc_free(p);
}

void pagealloc_free(char *v) {
   struct run *r;
/*
   if ((uint32_t)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
      panic("kfree");
*/
   // Fill with junk to catch dangling refs.
   //memset(v, 1, PGSIZE);


   r = (struct run *)v;
   r->next = kmem.freelist;
   kmem.freelist = r;

}

char *pagealloc_alloc(void) {
   struct run *r;

   r = kmem.freelist;
   if (r)
      kmem.freelist = r->next;

   return (char *)r;
}
