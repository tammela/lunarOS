#ifndef _COMPILER_H
#define _COMPILER_H

#define likely(x) (__builtin_expect(((x) != 0), 1))
#define unlikely(x) (__builtin_expect(((x) != 0), 0))

#define __inline __attribute__((always_inline)) inline

#define __printf(a, b) __attribute__((format(printf, 1, 2)))

#endif /* _COMPILER_H */
