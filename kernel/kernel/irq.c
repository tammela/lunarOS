#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/irq.h>
#include <lunaros/printf.h>

struct idt idt;

/* IDT table entries */
struct idt_descriptor entries[ISR_MAX] = {0};

/* Loads `idt` */
static __inline void lidt(void) {
   asm volatile("lidt %0" ::"m"(idt));
   asm volatile("sti");
}

int irq_init(void) {
   /* TODO: disable pic */
   /* TODO: enable lapic */
   idt.size = ISR_MAX;
   idt.addr = (uint64_t)&entries;
   lidt();
   return 0;
}
