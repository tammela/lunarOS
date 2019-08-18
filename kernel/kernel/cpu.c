#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

struct cpu cpu;

void cpu_init(void) {
   cpu_dofeatures();
   cpu_getinfo(&cpu);
}
