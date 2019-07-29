#include <std/stdint.h>

#include <lunaros/printf.h>
#include <lunaros/video/tty.h>

void main(uint64_t magic, uint64_t addr) {
   ((void)magic);
   ((void)addr);
   init_tty();
   puts("LunarOS Kernel\n");
}
