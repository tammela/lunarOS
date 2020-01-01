#include <builtin/cpuid.h>
#include <multiboot/multiboot2.h>
#include <std/stdint.h>
#include <std/stddef.h>

#include <lunaros/cpu.h>
#include <lunaros/irq.h>
#include <lunaros/mm.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/pagetable.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

void main(uint64_t magic, uint64_t addr, size_t physoff, pte_t *reserved) {
   physmem_layout_t *physmem_layout[MM_PHYSMEM_LAYOUT_SZ] = {NULL};
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid multiboot2 magic number");
      return;
   }
   puts("LunarOS Kernel\n");
   cpu_init();
   multiboot_parse_info(addr);
   multiboot_parse_mmap(addr, physmem_layout);
   mm_init(physmem_layout, physoff, reserved);
   irq_init(); /* must be after cpu_init() */
}
