#include <builtin/cpuid.h>
#include <multiboot/multiboot2.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/multiboot.h>
#include <lunaros/memlayout.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>
#include <lunaros/multiboot.h>
extern uint32_t _physical_end;
void main(uint32_t magic, uint32_t addr) {
   cls();
   if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
      puts("Invalid magic number");
      return;
   }
   puts("LunarOS Kernel\n");
   cpu_init();
   multiboot_parse(addr);
   mmap_print();
   mmap_for_each_region(&page_addrange);
   /* TEMP CODE: debug only */
   printf("0x%x%x \n ",(unsigned)((uint64_t)_physical_end >> 32),
      (unsigned)((uint64_t)_physical_end & 0xffffffff));
}
