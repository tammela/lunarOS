#include <std/stdint.h>
#include <multiboot/multiboot2.h>

#include <lunaros/printf.h>
#include <lunaros/video/tty.h>

void main(uint32_t magic, uint32_t addr) {
   ((void)addr);
   init_tty();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      printf("Invalid magic number");
      return;
   }
   puts("LunarOS Kernel\n");
}
