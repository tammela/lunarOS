#pragma once

struct cpu {
   uint8_t pmax; /* maximum physical address in bits */
   uint8_t lmax; /* maximum virtual address in bits */
};

void cpu_checkfeatures(void);
void cpu_setupmem(void);
void cpu_getinfo(struct cpu *cpu);
void cpu_init(void);
