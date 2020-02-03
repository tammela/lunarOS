#pragma once

/* arch dependent */
int _irq_init(void);

/* arch independent */
uint32_t irq_enabled(void);
void irq_init(void);
