#include <std/stdarg.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/printf.h>
#include <lunaros/video/tty.h>

static volatile uint16_t *video = NULL;

static int32_t columns = 0;
static int32_t lines = 0;

static void newline(void) {
   columns = 0;
   if (++lines >= TTY_MAX_HEIGHT)
      lines = 0;
}

static void putchar(char c) {
   size_t idx;
   if (c == '\n' || c == '\r') {
      newline();
      return;
   }
   idx = (lines * TTY_MAX_WIDTH) + columns;
   video[idx] = c | (DEFAULT_COLOR << 8);
   if (++columns >= TTY_MAX_WIDTH)
      newline();
}

static int uitoa(uintmax_t value, char *buf, int base) {
   static const char digits[] = "0123456789abcdef";
   int pos = 0;
   do {
      buf[pos++] = digits[value % base];
      value /= base;
   } while (value);
   /* string reverse */
   char tmp;
   for (int i = 0; i != pos / 2; i++) {
      tmp = buf[i];
      buf[i] = buf[pos - i - 1];
      buf[pos - i - 1] = tmp;
   }
   buf[pos] = '\0';
   return pos;
}

static void fmtnum(uintmax_t value, int base, uint8_t issigned) {
   char buffer[64];
   char sign = '\0';
   const char *baseprefix = "";
   const intmax_t svalue = value;
   switch (base) {
   case 2: {
      baseprefix = "0b";
      break;
   }
   case 8: {
      baseprefix = "0";
      break;
   }
   case 16: {
      baseprefix = "0x";
      break;
   }
   }
   if (issigned && svalue < 0) {
      sign = '-';
      uitoa(-svalue, buffer, base);
   } else {
      uitoa(value, buffer, base);
   }
   if (sign)
      putchar(sign);
   puts(baseprefix);
   puts(buffer);
}

void cls(void) {
   if (!video)
      video = (uint16_t *)TTY_MEM_ADDR;
   for (int32_t i = 0; i != TTY_MAX_HEIGHT * TTY_MAX_WIDTH; i++) {
      video[i] = ' ' | (DEFAULT_COLOR << 8);
   }
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
         case 'd':
         case 'i': {
            fmtnum(va_arg(args, int), 10, 1);
            break;
         }
         case 'o': {
            fmtnum(va_arg(args, unsigned int), 8, 0);
            break;
         }
         case 'x': {
            fmtnum(va_arg(args, unsigned int), 16, 0);
            break;
         }
         case 'u': {
            fmtnum(va_arg(args, unsigned int), 10, 0);
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
