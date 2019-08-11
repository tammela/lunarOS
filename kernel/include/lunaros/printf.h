#ifndef _PRINTF_H
#define _PRINTF_H

#include <std/stdint.h>
#include <std/stdarg.h>

#include <lunaros/compiler.h>

int uitoa(uintmax_t value, char *buf, int base);

void puts(const char *s);
void vprintf(const char *fmt, va_list args);
__printf(1, 2) void printf(const char *fmt, ...);

#endif /* _PRINTF_H */
