#ifndef _PRINTF_H
#define _PRINTF_H

#include <std/stdint.h>
#include <lunaros/compiler.h>

int uitoa(uintmax_t value, char *buf, int base);

void puts(const char *s);
__printf(1, 2) void printf(const char *fmt, ...);

#endif /* _PRINTF_H */
