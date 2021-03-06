#pragma once

/* Video memory address */
#ifndef TTY_MEM_ADDR
#define TTY_MEM_ADDR    (0xB8000)
#endif /* TTY_MEM_ADDR */

#ifndef TTY_MAX_WIDTH
#define TTY_MAX_WIDTH   (80)
#endif /* TTY_MAX_WIDTH */

#ifndef TTY_MAX_HEIGHT
#define TTY_MAX_HEIGHT  (24)
#endif /* TTY_MAX_HEIGHT */

/* White foreground, black background */
#ifndef DEFAULT_COLOR
#define DEFAULT_COLOR   (0x0F)
#endif /* DEFAULT_COLOR */

void putchar(char c);
void cls(void);
