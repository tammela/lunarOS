#pragma once

/* Juice up the branch predictor */
#define likely(x)       (__builtin_expect(((x) != 0), 1))
#define unlikely(x)     (__builtin_expect(((x) != 0), 0))

/* Useful GCC attributes */
#define __aligned       __attribute__((aligned))
#define __interrupt     __attribute__((interrupt))
#define __inline        __attribute__((always_inline)) inline
#define __noreturn      __attribute__((noreturn))
#define __packed        __attribute__((packed))
#define __printf(a, b)  __attribute__((format(printf, 1, 2)))
