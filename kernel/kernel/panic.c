#include <lunaros/compiler.h>
#include <lunaros/printf.h>

#include <lunaros/x86.h>

void __noreturn panic(const char *fmt, ...) {
   va_list args;
   va_start(args, fmt);
   printf("[PANIC] ");
   vprintf(fmt, args);
   va_end(args);
   cli();
   while (1) {
   }
}
