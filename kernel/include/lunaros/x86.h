#ifndef _X86_H
#define _X86_H

#include <lunaros/compiler.h>

static __inline void outb(uint16_t port, uint8_t val) {
      asm volatile("outb %0, %1" ::"a"(val), "Nd"(port));
}

#endif /* _X86_H */
