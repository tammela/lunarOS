#pragma once

#include <std/stddef.h>

#include <lunaros/compiler.h>

#include <arch/x86/irq.h>

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
static __inline void lidt(gate_t *entries, uint16_t size) {
   static volatile idt_t idt;
   idt.size = size;
   idt.addr = (uint64_t)entries;
   asm volatile("lidt %0" ::"r"(idt));
}

static __inline void rdmsr(uint32_t reg, uint32_t *low, uint32_t *high) {
   asm volatile("rdmsr" : "=a"(*low), "=d"(*high) : "c"(reg));
}

static __inline void wrmsr(uint32_t reg, uint64_t val) {
   uint32_t ina, ind;
   ina = val & 0xFFFFFFFF;
   ind = val & (0xFFFFFFFFll << 32);
   asm volatile("wrmsr" : : "c"(reg), "a"(ina), "d"(ind));
}

static __inline void invlpg(uint64_t addr) {
   asm volatile("invlpg (%0)" : : "r"(addr) : "memory" );
}
