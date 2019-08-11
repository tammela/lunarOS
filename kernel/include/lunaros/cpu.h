#ifndef _CPU_H
#define _CPU_H

/*
** Per-arch function to check for the support of
** _required_ features. Defined at `arch/`.
*/
void cpu_dofeatures(void);

void cpu_init(void);

#endif /* _CPU_H */
