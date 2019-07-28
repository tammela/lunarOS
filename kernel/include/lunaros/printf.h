#ifndef _PRINTF_H
#define _PRINTF_H

#include <lunaros/compiler.h>

void puts(const char *s);
__printf(1, 2) void printf(const char *fmt, ...);

#endif /* _PRINTF_H */
