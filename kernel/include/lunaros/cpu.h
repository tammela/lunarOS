#ifndef _CPU_H
#define _CPU_H

struct cpu {
   uint8_t pmax;      /* maximum physical address */
   uint8_t lmax;      /* maximum virtual address */
};

/*
** Per-arch function to check for the support of
** _required_ features. Defined at `arch/`.
*/
void cpu_dofeatures(void);
void cpu_getinfo(struct cpu *cpu);
void cpu_init(void);

#endif /* _CPU_H */
