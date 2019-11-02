#pragma once

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

enum {
   ISR_GATETYPE_INTERRUPT = 0x0E,
   ISR_GATETYPE_TRAP = 0x0F
};

/*
** Intel manual specifies a maximum of 256 ISRs
*/
#define ISR_MAX (256)
