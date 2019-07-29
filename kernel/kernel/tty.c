#include <std/stdarg.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/printf.h>
#include <lunaros/video/tty.h>

static volatile uint16_t *video;

static int32_t columns = 0;
static int32_t lines = 0;

static void newline(void) {
   columns = 0;
   if (++lines >= TTY_MAX_HEIGHT)
      lines = 0;
}

static void putchar(char c) {
   size_t idx;
   if (c == '\n') {
      newline();
      return;
   }
   idx = (lines * TTY_MAX_HEIGHT) + columns;
   video[idx] = c | (0x0F << 8);
   if (++columns >= TTY_MAX_WIDTH)
      newline();
}

void puts(const char *s) {
   while (*s != '\0') putchar(*s++);
}

__printf(1, 2) void printf(const char *fmt, ...) {
   va_list args;
   char c;
   va_start(args, fmt);
   while ((c = *fmt++) != '\0') {
      if (c != '%')
         putchar(c);
      else
         switch (*fmt++) {
         case 's': {
            char *p = va_arg(args, char *);
            if (!p)
               p = "(null)";
            puts(p);
            break;
         }
         default: {
            putchar(*fmt++);
            break;
         }
         }
   }
   va_end(args);
}

void init_tty(void) {
   video = (uint16_t *)TTY_MEM_ADDR;
}
