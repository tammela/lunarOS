#include <builtin/cpuid.h>
#include <multiboot/multiboot2.h>
#include <std/stdint.h>
#include <std/stddef.h>

#include <lunaros/cpu.h>
#include <lunaros/irq.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

/* let's assume we have at least 8 layouts of available memory */
physmem_layout_t *physmem_layout[8] = {NULL};

void main(uint64_t magic, uint64_t addr) {
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid multiboot2 magic number");
      return;
   }
   puts("LunarOS Kernel\n");
   cpu_init();
   multiboot_parse_mmap(addr);
   irq_init(); /* must be after cpu_init() */
   multiboot_parse_info(addr);
}
