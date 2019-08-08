#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

void multiboot_mmap_iterate(unsigned long addr);
void multiboot_print_info(unsigned long addr);

#endif /* _MULTIBOOT_H */
