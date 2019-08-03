#include <std/stdarg.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/tty.h>

static volatile uint16_t *video = (uint16_t *)TTY_MEM_ADDR;

static int32_t columns = 0;
static int32_t lines = 0;

static void newline(void) {
   columns = 0;
   if (++lines >= TTY_MAX_HEIGHT) {
      lines = 0;
      cls();
   }
}

void putchar(char c) {
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

void cls(void) {
   for (int32_t i = 0; i != TTY_MAX_HEIGHT * TTY_MAX_WIDTH; i++) {
      video[i] = ' ' | (DEFAULT_COLOR << 8);
   }
}
