#include <lunaros/stdint.h>

void main(uint64_t magic, uint64_t addr) {
   unsigned char *vga = (unsigned char *)0xb8000;
   ((void)magic);
   ((void)addr);

   vga[0] = '.';
   vga[1] = 4;

   while (1)
      ;
}
