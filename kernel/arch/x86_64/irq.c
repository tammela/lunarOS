#include <lunaros/irq.h>
#include <lunaros/x86.h>

#define IO_PIC1 (0x20)  /* Master PIC base */
#define IO_PIC2 (0xA0)  /* Slave PIC base */

/*
** Disable all legacy features, like PIC.
*/
void irq_dolegacy(void) {
   /* Disable PIC */
   outb(IO_PIC1 + 1, 0xFF);
   outb(IO_PIC2 + 1, 0xFF);
}
