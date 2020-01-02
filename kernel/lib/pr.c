#include <lunaros/printf.h>

void pr_debug(const char *fmt, ...) {
#ifdef __DEBUG__
   va_list args;
   puts("[DEBUG] ");
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
#endif
}

void pr_err(const char *fmt, ...) {
   va_list args;
   puts("[ERROR] ");
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
}
