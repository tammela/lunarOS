#include <multiboot/multiboot2.h>
#include <std/stdint.h>

#include <lunaros/printf.h>
#include <lunaros/video/tty.h>

void main(uint32_t magic, uint32_t addr) {
   ((void)addr);
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid magic number");
      return;
   }
   puts("LunarOS Kernel\n");
}
