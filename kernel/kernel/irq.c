#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/irq.h>
#include <lunaros/kernel.h>

void irq_init(void) {
   if (!_irq_init())
      panic("Failed to setup interrupts");
}
