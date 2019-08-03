#include <std/stdarg.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

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
