#ifndef _TTY_H
#define _TTY_H

/* Video memory address */
#ifndef TTY_MEM_ADDR_
#define TTY_MEM_ADDR (0xb8000)
#endif /* TTY_MEM_ADDR */

#ifndef TTY_MAX_WIDTH
#define TTY_MAX_WIDTH (80)
#endif /* TTY_MAX_WIDTH */

#ifndef TTY_MAX_HEIGHT
#define TTY_MAX_HEIGHT (24)
#endif /* TTY_MAX_HEIGHT */

void init_tty(void);
void clrscr(void);

#endif /* _TTY_H */
