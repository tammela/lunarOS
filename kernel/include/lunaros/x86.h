#ifndef _X86_H
#define _X86_H

#include <std/stddef.h>

#include <lunaros/irq.h>
#include <lunaros/compiler.h>

static __inline void outb(uint16_t port, uint8_t val) {
   asm volatile("outb %0, %1" ::"a"(val), "Nd"(port));
}

/*
** Start all interrupts from the CPU
*/
static __inline void sti(void) {
   asm volatile("sti");
}

/*
** Stop all interrupts from the CPU
*/
static __inline void cli(void) {
   asm volatile("cli");
}

/*
** Loads the interrupt descriptor table
*/
static __inline void lidt(struct gate *entries, uint16_t size) {
   volatile struct idt idt;
   idt.size = size;
   idt.addr = (uint64_t)entries;
   asm volatile("lidt %0" ::"r"(idt));
}

static __inline void rdmsr(uint32_t reg, uint32_t *low, uint32_t *high) {
   asm volatile("rdmsr"
                : "=a"(*low), "=d"(*high)
                : "c"(reg)
   );
}

static __inline void wrmsr(uint32_t reg, uint64_t val) {
   uint32_t ina, ind;
   ina = val & 0xFFFFFFFF;
   ind = val & (0xFFFFFFFFll << 32);
   asm volatile("wrmsr"
                :
                : "c"(reg), "a"(ina), "d"(ind)
   );
}

#endif /* _X86_H */
