#pragma once

#include <std/stdarg.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>

int uitoa(uintmax_t value, char *buf, int base);

void puts(const char *s);
void vprintf(const char *fmt, va_list args);
__printf(1, 2) void printf(const char *fmt, ...);

void pr_debug(const char *fmt, ...);
void pr_warn(const char *fmt, ...);
void pr_err(const char *fmt, ...);
