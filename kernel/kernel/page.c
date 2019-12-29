#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/kernel.h>
#include <lunaros/mm.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>

/*
** Reserved memory for elf
*/
static size_t physoff;

void page_init(physmem_layout_t **layouts, size_t poff) {
   physoff = poff;
   for (int i = 0; i < MM_PHYSMEM_LAYOUT_SZ; i++) {
      if (layouts[i] == NULL)
         continue;

      uint64_t minaddr = PGROUNDUP(layouts[i]->addr, PGSIZE);
      uint64_t maxaddr = PGROUNDDOWN(layouts[i]->addr + layouts[i]->len, PGSIZE);

      (void)minaddr;
      (void)maxaddr;

      /* TODO: create buddy for area */
   }
}
