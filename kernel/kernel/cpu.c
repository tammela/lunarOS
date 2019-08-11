#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

void cpu_init(void) {
   cpu_dofeatures();
}
