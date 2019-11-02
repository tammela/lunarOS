#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/irq.h>
#include <lunaros/kernel.h>

void irq_init(void) {
   irq_dolegacy();
   if (!irq_setuparch())
      panic("Failed to setup interrupts");
}
