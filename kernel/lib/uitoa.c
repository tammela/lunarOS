#include <lunaros/printf.h>
#include <std/stdint.h>

int uitoa(uintmax_t value, char *buf, int base) {
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
