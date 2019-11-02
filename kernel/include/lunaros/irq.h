#pragma once

#include <std/stdint.h>
#include <lunaros/compiler.h>

struct idt {
   uint16_t size; /* entries count */
   uint64_t addr; /* starting entry address */
} __packed;

struct gate {
   uint16_t low_addr;   /* low address offset */
   uint16_t selector;   /* segment selector */
   uint8_t ist;         /* interrupt stack table [0..2] */
   uint8_t type : 4;    /* gate type */
   uint8_t zero : 1;    /* always zero */
   uint8_t dpl : 2;     /* descriptor privilege level */
   uint8_t present : 1; /* segment present */
   uint16_t mid_addr;   /* mid address offset */
   uint32_t high_addr;  /* high address offset */
   uint32_t reserved;   /* reserved */
} __packed;

/* ISR gate types */
#define INTERRUPT_GATE  (0x0E)
#define TRAP_GATE       (0x0F)

/* Intel manual specifies a maximum of 256 ISRs */
#define ISR_MAX (256)

int irq_setuparch(void);
void irq_dolegacy(void);
void irq_init(void);
