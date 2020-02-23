#pragma once

#include <std/stddef.h>

#include <lunaros/compiler.h>

/* See docs/virtual-memory.txt */
#define _VM_KERNEL_START        (0xFFFF800000000000)

void __noreturn panic(const char *fmt, ...);

/* container_of() from NetBSD */

#define __validate_container_of(PTR, TYPE, FIELD)                 \
   (0 * sizeof((PTR) - &((TYPE *)(((char *)(PTR)) -               \
      offsetof(TYPE, FIELD)))->FIELD))
#define __validate_const_container_of(PTR, TYPE, FIELD)           \
   (0 * sizeof((PTR) - &((const TYPE *)(((const char *)(PTR)) -   \
      offsetof(TYPE, FIELD)))->FIELD))

#define container_of(PTR, TYPE, FIELD)                            \
   ((TYPE *)(((char *)(PTR)) - offsetof(TYPE, FIELD))             \
       + __validate_container_of(PTR, TYPE, FIELD))
#define const_container_of(PTR, TYPE, FIELD)                      \
   ((const TYPE *)(((const char *)(PTR)) - offsetof(TYPE, FIELD)) \
       + __validate_const_container_of(PTR, TYPE, FIELD))

static __inline uint64_t log2(uint64_t n) {
   if (n == 0) return 0;
   return __builtin_ctzll(n);
}
static __inline uint64_t npow2(uint64_t n) {
   return n == 1 ? 1 : 1 << (64 - __builtin_clzl(n - 1));
}
