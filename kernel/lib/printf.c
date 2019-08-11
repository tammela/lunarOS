#include <lunaros/compiler.h>
#include <lunaros/printf.h>

__printf(1, 2) void printf(const char *fmt, ...) {
   va_list args;
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
}
