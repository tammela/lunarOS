#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/memlayout.h>
#include <lunaros/printf.h>
#include <lunaros/x86.h>

#define MMAP_MAX_COUNT  10
#define MAX_CMDLINE_LEN 256

const char *cmdline;
static struct mmap_entry memory_map[MMAP_MAX_COUNT];
static size_t mmap_size;

unsigned long initrd_begin;
unsigned long initrd_end;

void mmap_add_region(uint64_t addr, uint64_t length) {
   memory_map[mmap_size].addr = addr;
   memory_map[mmap_size].length = length;
   mmap_size++;
}

void mmap_for_each_region(region_fptr_t exec) {
   struct mmap_entry entry;
   for (size_t idx = 0; idx < mmap_size; idx++) {
      entry = memory_map[idx];
      exec(entry.addr, entry.length);
   }
}

static void print_entry(uint64_t addr, uint64_t length) {
   printf(" base_addr = 0x%x%x,"
      " length = 0x%x%x\n",
      (unsigned)(addr >> 32),
      (unsigned)(addr & 0xffffffff),
      (unsigned)(length >> 32),
      (unsigned)(length & 0xffffffff));
}

void mmap_print(void) {
   printf("mmap\n");
   mmap_for_each_region(&print_entry);
}