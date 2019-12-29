#pragma once

#include <lunaros/page.h>

void multiboot_parse_mmap(unsigned long addr, physmem_layout_t **layouts);
void multiboot_parse_info(unsigned long addr);
