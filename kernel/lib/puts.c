#include <lunaros/tty.h>

void puts(const char *s) {
   while (*s != '\0') putchar(*s++);
}
