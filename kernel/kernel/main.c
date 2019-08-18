#include <builtin/cpuid.h>
#include <multiboot/multiboot2.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/irq.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

void main(uint32_t magic, uint32_t addr) {
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid multiboot2 magic number");
      return;
   }
   puts("LunarOS Kernel\n");
   cpu_init();
   irq_init(); /* must be after cpu_init() */
   multiboot_print_info(addr);
}
