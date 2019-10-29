#include <std/stdint.h>
#include <std/stddef.h>

#include <lunaros/cpu.h>
#include <lunaros/irq.h>
#include <lunaros/kernel.h>
#include <lunaros/x86.h>

#include <arch/x86/msr.h>

#include "traps.h"

extern struct cpu cpu;  /* CPU info */

#define IO_PIC1 (0x20)  /* Master PIC base */
#define IO_PIC2 (0xA0)  /* Slave PIC base */

volatile uint32_t *lapic;

/*
** Disable all legacy features, like PIC.
*/
void irq_dolegacy(void) {
   /* Disable PIC */
   outb(IO_PIC1 + 1, 0xFF);
   outb(IO_PIC2 + 1, 0xFF);
}

int lapic_search(void) {
   uint32_t low, high;
   uint32_t mask = 1;
   rdmsr(IA32_APIC_BASE, &low, &high);
   uint64_t addr = low & 0xFFFFF000; /* fixed 20 bits in lower part */
   uint32_t remaining = cpu.pmax - 20;
   /* max physical address is variable */
   while (high && remaining) {
      mask <<= 1;
      mask ^= 1;
      remaining--;
   }
   addr |= ((uint64_t)(high & mask)) << 32;
   /* Intel manual says lapic is always mapped to 0xFEE00000 in physical mem */
   return !!(lapic = (uint32_t *)addr);
   /* TODO: map lapic to strong uncacheable virtual map */
}

/* lapic registers - from xv6 */
#define ID      (0x0020/4)      /* ID */
#define VER     (0x0030/4)      /* Version */
#define TPR     (0x0080/4)      /* Task Priority */
#define EOI     (0x00B0/4)      /* EOI */
#define SVR     (0x00F0/4)      /* Spurious Interrupt Vector */
  #define ENABLE 0x00000100     /* Unit Enable */
#define ESR     (0x0280/4)      /* Error Status */
#define ICRLO   (0x0300/4)      /* Interrupt Command */
  #define INIT       0x00000500 /* INIT/RESET */
  #define STARTUP    0x00000600 /* Startup IPI */
  #define DELIVS     0x00001000 /* Delivery status */
  #define ASSERT     0x00004000 /* Assert interrupt (vs deassert) */
  #define DEASSERT   0x00000000
  #define LEVEL      0x00008000 /* Level triggered */
  #define BCAST      0x00080000 /* Send to all APICs, including self. */
  #define BUSY       0x00001000
  #define FIXED      0x00000000
#define ICRHI   (0x0310/4)      /* Interrupt Command [63:32] */
#define TIMER   (0x0320/4)      /* Local Vector Table 0 (TIMER) */
  #define X1         0x0000000B /* Divide counts by 1 */
  #define PERIODIC   0x00020000 /* Periodic */
#define PCINT   (0x0340/4)      /* Performance Counter LVT */
#define LINT0   (0x0350/4)      /* Local Vector Table 1 (LINT0) */
#define LINT1   (0x0360/4)      /* Local Vector Table 2 (LINT1) */
#define ERROR   (0x0370/4)      /* Local Vector Table 3 (ERROR) */
  #define MASKED     0x00010000 /* Interrupt masked */
#define TICR    (0x0380/4)      /* Timer Initial Count */
#define TCCR    (0x0390/4)      /* Timer Current Count */
#define TDCR    (0x03E0/4)      /* Timer Divide Configuration */

static void lapicw(int idx, int v) {
   lapic[idx] = v;
   (void)(lapic[ID]); /* wait write to finish */
}

/*
** Setup lapic - always assuming SMP hardware
*/
int irq_setuparch(void) {
   if (unlikely(!lapic_search()))
      panic("Failed to setup lapic");
   lapicw(SVR, ENABLE | (T_IRQ0 + IRQ_SPURIOUS));
   return 0;
}
