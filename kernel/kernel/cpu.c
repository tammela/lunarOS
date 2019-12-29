#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

static struct cpu cpu;

uint8_t cpu_max_physical(void) {
   return cpu.pmax;
}

void cpu_init(void) {
   cpu_checkfeatures();
   cpu_getinfo(&cpu);
   cpu_setupmem();
}
