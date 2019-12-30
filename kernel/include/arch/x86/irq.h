#pragma once

#include <lunaros/compiler.h>

typedef struct idt_t idt_t;

struct idt_t {
   uint16_t size; /* entries count */
   uint64_t addr; /* starting entry address */
} __packed;

typedef struct gate_t gate_t;

struct gate_t {
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

typedef enum gt_t gt_t;

/* Gate type */
enum gt_t {
   ISR_INTERRUPT = 0x0E,
   ISR_TRAP = 0x0F
};

/*
** Intel manual specifies a maximum of 256 ISRs
*/
#define ISR_MAX (256)
