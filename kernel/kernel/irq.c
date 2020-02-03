#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/irq.h>
#include <lunaros/kernel.h>

static uint32_t enabled = 0;

uint32_t irq_enabled(void) {
   return enabled;
}

void irq_init(void) {
   if (!_irq_init())
      panic("Failed to setup interrupts");
   enabled = 1;
}
