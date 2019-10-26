#ifndef _PAGETABLE_H
#define _PAGETABLE_H

#include <std/stdint.h>

/* Defined on boot time by boot.S */
extern uint64_t pml4[512];
extern uint64_t low_pdpte[512];
extern uint64_t high_pdpte[512];
extern uint64_t high_pde[512];
extern uint64_t low_pde[512];


#endif /* _PAGETABLE_H */
