#include <multiboot/multiboot2.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/multiboot.h>
#include <lunaros/memlayout.h>
#include <lunaros/printf.h>

void multiboot_parse(unsigned long addr) {
   struct multiboot_tag *tag;

   if (addr & 7) {
      printf("WARN Unaligned mbi\n");
      return;
   }

   for (tag = (struct multiboot_tag *)(addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7))) {
      switch (tag->type) {
      /* later, we can save also cmdline params, initrd location in memory (information about initrd in multiboot modules)
      case MULTIBOOT_TAG_TYPE_CMDLINE:
         printf("Command line = %s\n",
                ((struct multiboot_tag_string *)tag)->string);
         break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
         printf("Boot loader name = %s\n",
                ((struct multiboot_tag_string *)tag)->string);
         break;
      case MULTIBOOT_TAG_TYPE_MODULE:
         printf("Module at 0x%x-0x%x. Command line %s\n",
                ((struct multiboot_tag_module *)tag)->mod_start,
                ((struct multiboot_tag_module *)tag)->mod_end,
                ((struct multiboot_tag_module *)tag)->cmdline);
         break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
         printf("mem_lower = %uKB, mem_upper = %uKB\n",
                ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower,
                ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper);
         break;
      */
      case MULTIBOOT_TAG_TYPE_MMAP: {
         multiboot_memory_map_t *mmap;

         for (mmap = ((struct multiboot_tag_mmap *)tag)->entries;
              (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)tag + tag->size;
              mmap = (multiboot_memory_map_t*)((unsigned long)mmap + ((struct multiboot_tag_mmap *)tag)->entry_size)) {
            /* only type 1 means "available RAM" */
            if (mmap->type != 1) continue;
            /* add available RAM region to our memory map */
            mmap_add_region(mmap->addr, mmap->len);
         }
      } break;
      }
   }
}
