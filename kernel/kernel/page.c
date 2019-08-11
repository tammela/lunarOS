#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/kernel.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>

/*
** Set on `boot.S` in the .code64 section.
** All addresses below are guaranteed to be
** identitiy mapped on main entry by `boot.S`.
** Must not allow freeing addresses below this!!
*/
extern char *_physical_end;

struct {
   struct page *free; /* single-linked list of free pages */
} kmem;

void page_addrange(void *start, void *end) {
   page_freerange(start, end);
}

void page_freerange(void *vstart, void *vend) {
   char *p;
   p = (char *)PGROUNDUP((uintptr_t)vstart);
   for (; p + PGSIZE <= (char *)vend; p += PGSIZE) page_free(p);
}

void page_free(void *page) {
   struct page *p;
   if (unlikely((uintptr_t)page < (uintptr_t)_physical_end))
      panic("page_free");
   p = (struct page *)page;
   p->next = kmem.free;
   kmem.free = p;
}

void *page_alloc(void) {
   struct page *p;
   p = kmem.free;
   if (p)
      kmem.free = p->next;
   return (void *)p;
}

void page_init(void *vstart, void *vend) {
   page_freerange(vstart, vend);
}
