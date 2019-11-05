#include <multiboot/multiboot2.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/multiboot.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>

#define next_tag(t) \
   ((struct multiboot_tag *)((multiboot_uint8_t *)t + ((t->size + 7) & ~7)))

#define next_mmap_tag(m, t) \
   ((uint8_t *)m + ((struct multiboot_tag_mmap *)t)->entry_size)

void multiboot_mmap_iterate(unsigned long addr) {
   struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);

   if (addr & 7) {
      printf("WARN Unaligned mbi\n");
      return;
   }

   for (; tag->type != MULTIBOOT_TAG_TYPE_END; tag = next_tag(tag)) {
      if (tag->type != MULTIBOOT_TAG_TYPE_MMAP)
         continue;
      multiboot_memory_map_t *mmap =
          ((struct multiboot_tag_mmap *)tag)->entries;
      while ((uint8_t *)mmap < ((uint8_t *)tag + tag->size)) {
         mmap = (multiboot_memory_map_t *)next_mmap_tag(mmap, tag);
      }
   }
}

void multiboot_print_info(unsigned long addr) {
   struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);

   if (addr & 7) {
      printf("WARN Unaligned mbi\n");
      return;
   }

   for (; tag->type != MULTIBOOT_TAG_TYPE_END; tag = next_tag(tag)) {
      switch (tag->type) {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
         printf("Command line = %s\n",
                ((struct multiboot_tag_string *)tag)->string);
         break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
         printf("Boot loader name = %s\n",
                ((struct multiboot_tag_string *)tag)->string);
         break;
      case MULTIBOOT_TAG_TYPE_MODULE:
         printf("Module at %x-%x. Command line %s\n",
                ((struct multiboot_tag_module *)tag)->mod_start,
                ((struct multiboot_tag_module *)tag)->mod_end,
                ((struct multiboot_tag_module *)tag)->cmdline);
         break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
         printf("mem_lower = %uKB, mem_upper = %uKB\n",
                ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower,
                ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper);
         break;
      case MULTIBOOT_TAG_TYPE_MMAP: {
         multiboot_memory_map_t *mmap =
             ((struct multiboot_tag_mmap *)tag)->entries;
         while ((uint8_t *)mmap < ((uint8_t *)tag + tag->size)) {
            printf(" base_addr = %llx, length = %llx, type = %x\n", mmap->addr,
                   mmap->len, mmap->type);
            mmap = (multiboot_memory_map_t *)next_mmap_tag(mmap, tag);
         }
         break;
      }
      }
   }
}
