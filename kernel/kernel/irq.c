#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/irq.h>
#include <lunaros/kernel.h>
#include <lunaros/printf.h>
#include <lunaros/x86.h>

/* IDT table entries */
struct gate entries[ISR_MAX] = {0};

void irq_init(void) {
   irq_dolegacy();
   if (!irq_setuparch())
      panic("Failed to setup interrupts");
}
