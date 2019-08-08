#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

void multiboot_mmap_iterate(unsigned long magic, unsigned long addr, void (*exec)(unsigned long saddr, unsigned long eaddr));
void multiboot_print_info(unsigned long magic, unsigned long addr);

#endif /* _MULTIBOOT_H */
