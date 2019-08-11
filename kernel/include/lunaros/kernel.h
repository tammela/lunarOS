#ifndef _KERNEL_H
#define _KERNEL_H

#include <lunaros/compiler.h>

void __noreturn panic(const char *fmt, ...);

#endif /* _KERNEL_H */
