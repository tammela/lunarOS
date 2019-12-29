#pragma once

struct cpu {
   uint8_t pmax; /* maximum physical address in bits */
   uint8_t lmax; /* maximum virtual address in bits */
};

/* arch defined */
void cpu_checkfeatures(void);
void cpu_setupmem(void);
void cpu_getinfo(struct cpu *cpu);

/* arch independent */
uint8_t cpu_max_physical(void);
void cpu_init(void);
