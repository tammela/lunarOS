#include <multiboot/multiboot2.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/kernel.h>
#include <lunaros/multiboot.h>
#include <lunaros/mm.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>

#define next_tag(t) \
   ((struct multiboot_tag *)((multiboot_uint8_t *)t + ((t->size + 7) & ~7)))

#define next_mmap_tag(m, t) \
   ((uint8_t *)m + ((struct multiboot_tag_mmap *)t)->entry_size)

size_t count_mem_available(uint64_t addr) {
   multiboot_memory_map_t *mmap = NULL;
   size_t areas_sz = 0;
   struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);
   for (; tag->type != MULTIBOOT_TAG_TYPE_END; tag = next_tag(tag)) {
      if (tag->type != MULTIBOOT_TAG_TYPE_MMAP)
         continue;
      mmap = ((struct multiboot_tag_mmap *)tag)->entries;
      while ((uint8_t *)mmap < ((uint8_t *)tag + tag->size)) {
         if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
            areas_sz++;
         mmap = (multiboot_memory_map_t *)next_mmap_tag(mmap, tag);
      }
   }
   return areas_sz;
}

void fill_area_info(uint64_t addr, mem_area_t *areas) {
   multiboot_memory_map_t *mmap = NULL;
   struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);
   for (size_t i = 0; tag->type != MULTIBOOT_TAG_TYPE_END; tag = next_tag(tag)) {
      if (tag->type != MULTIBOOT_TAG_TYPE_MMAP)
         continue;
      mmap = ((struct multiboot_tag_mmap *)tag)->entries;
      while ((uint8_t *)mmap < ((uint8_t *)tag + tag->size)) {
         if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            areas[i].addr = mmap->addr;
            ALIGN_TO(areas[i].addr, sizeof(uintmax_t));
            areas[i].len = mmap->len;
            i++;
         }
         mmap = (multiboot_memory_map_t *)next_mmap_tag(mmap, tag);
      }
   }
}

mem_area_t *multiboot_parse_mmap(uint64_t addr, size_t *sz) {
   mem_area_t *areas = NULL;
   size_t areas_sz = count_mem_available(addr);
   areas = mm_reserved_alloc(areas_sz * sizeof(mem_area_t));
   if (unlikely(areas == NULL))
      panic("Not enough memory\n");
   fill_area_info(addr, areas);
   *sz = areas_sz;
   return areas;
}

void multiboot_parse_info(uint64_t addr) {
   struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);
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
