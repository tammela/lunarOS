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

void fmtnum64(const char *fmt, va_list args) {
   switch (*fmt) {
   case 'd':
   case 'i': {
      fmtnum(va_arg(args, uint64_t), 10, 1);
      break;
   }
   case 'o': {
      fmtnum(va_arg(args, uint64_t), 8, 0);
      break;
   }
   case 'x': {
      fmtnum(va_arg(args, uint64_t), 16, 0);
      break;
   }
   case 'u': {
      fmtnum(va_arg(args, uint64_t), 10, 0);
      break;
   }
   default: {
      break;
   }
   }
}

void fmtnum32(const char *fmt, va_list args) {
   switch (*fmt) {
   case 'd':
   case 'i': {
      fmtnum(va_arg(args, int32_t), 10, 1);
      break;
   }
   case 'o': {
      fmtnum(va_arg(args, uint32_t), 8, 0);
      break;
   }
   case 'x': {
      fmtnum(va_arg(args, uint32_t), 16, 0);
      break;
   }
   case 'u': {
      fmtnum(va_arg(args, uint32_t), 10, 0);
      break;
   }
   default: {
      break;
   }
   }
}

void vprintf(const char *fmt, va_list args) {
   char c;
   while ((c = *fmt++) != '\0') {
      if (c != '%')
         putchar(c);
      else
         switch (*fmt) {
         case 'l': {
            if (*(fmt + 1) != 'l')
               fmtnum64(++fmt, args);
            else {
               fmt += 2; /* skip 'll' */
               fmtnum64(fmt++, args);
            }
            break;
         }
         case 's': {
            char *p = va_arg(args, char *);
            if (!p)
               p = "(null)";
            puts(p);
            fmt++;
            break;
         }
         case 'd':
         case 'i':
         case 'o':
         case 'x':
         case 'u': {
            fmtnum32(fmt++, args);
            break;
         }
         default: {
            putchar(*fmt++);
            break;
         }
         }
   }
}
