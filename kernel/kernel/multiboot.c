#include <std/stddef.h>
#include <std/stdint.h>
#include <multiboot/multiboot2.h>

#include <lunaros/multiboot.h>
#include <lunaros/printf.h>

void multiboot_mmap_iterate(unsigned long magic, unsigned long addr, void (*exec)(unsigned long saddr, unsigned long eaddr)) {
   ((void)magic);
   struct multiboot_tag *tag;

   if (addr & 7) {
      printf("WARN Unaligned mbi\n");
      return;
   }

   for (tag = (struct multiboot_tag *)(addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                       ((tag->size + 7) & ~7))) {
         if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            multiboot_memory_map_t *mmap;
            for (mmap = ((struct multiboot_tag_mmap *)tag)->entries;
               (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)tag + tag->size;
               mmap = (multiboot_memory_map_t
                           *)((unsigned long)mmap +
                              ((struct multiboot_tag_mmap *)tag)->entry_size)) {
               printf(" base_addr = 0x%x%x,"
                   " length = 0x%x%x, type = 0x%x\n",
                   (unsigned)(mmap->addr >> 32),
                   (unsigned)(mmap->addr & 0xffffffff),
                   (unsigned)(mmap->len >> 32),
                   (unsigned)(mmap->len & 0xffffffff), (unsigned)mmap->type);
               (void)exec;
               exec(mmap->addr, mmap->addr + mmap->len);
            }
         }
   }
}

void multiboot_print_info(unsigned long magic, unsigned long addr) {
   ((void)magic);
   struct multiboot_tag *tag;
   unsigned size;

   if (addr & 7) {
      printf("WARN Unaligned mbi\n");
      return;
   }

   size = *(unsigned *)addr;
   printf("Announced mbi size 0x%x\n", size);
   for (tag = (struct multiboot_tag *)(addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                       ((tag->size + 7) & ~7))) {
      //printf("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
      
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
      case MULTIBOOT_TAG_TYPE_MMAP: {
         multiboot_memory_map_t *mmap;

         printf("mmap\n");

         for (mmap = ((struct multiboot_tag_mmap *)tag)->entries;
              (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)tag + tag->size;
              mmap = (multiboot_memory_map_t
                          *)((unsigned long)mmap +
                             ((struct multiboot_tag_mmap *)tag)->entry_size))
            printf(" base_addr = 0x%x%x,"
                   " length = 0x%x%x, type = 0x%x\n",
                   (unsigned)(mmap->addr >> 32),
                   (unsigned)(mmap->addr & 0xffffffff),
                   (unsigned)(mmap->len >> 32),
                   (unsigned)(mmap->len & 0xffffffff), (unsigned)mmap->type);
      } break;
      }
      
   }
   //tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
   //                               ((tag->size + 7) & ~7));
   //printf("Total mbi size 0x%x\n", (unsigned)tag - addr);
}