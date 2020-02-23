#include <builtin/cpuid.h>
#include <multiboot/multiboot2.h>
#include <std/stdint.h>
#include <std/stddef.h>

#include <lunaros/cpu.h>
#include <lunaros/irq.h>
#include <lunaros/kernel.h>
#include <lunaros/mm.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/pagetable.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

void main(uint64_t magic, uint64_t addr, size_t physoff, pte_t *reserved) {
   physmem_layout_t *layouts[MM_PHYSMEM_LAYOUT_SZ] = {NULL};
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid multiboot2 magic number");
      return;
   }
   puts("LunarOS Kernel\n");
   ALIGN_TO(physoff, 8);
   cpu_init();
   multiboot_parse_info(addr);
   multiboot_parse_mmap(addr, layouts);
   mm_init(layouts, physoff, reserved);
   irq_init(); /* must be after cpu_init() */
}
