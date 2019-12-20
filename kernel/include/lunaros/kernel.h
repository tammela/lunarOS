#pragma once

#include <lunaros/compiler.h>

/* See docs/virtual-memory.txt */
#define _VM_KERNEL_START        (0xFFFF800000000000)

void __noreturn panic(const char *fmt, ...);
