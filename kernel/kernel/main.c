#include <builtin/cpuid.h>
#include <multiboot/multiboot2.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>
#include <lunaros/multiboot.h>

void main(uint32_t magic, uint32_t addr) {
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid magic number");
      return;
   }
   puts("LunarOS Kernel\n");
   cpu_init();
   multiboot_print_info(addr);
}
