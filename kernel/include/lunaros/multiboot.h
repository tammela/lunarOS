#pragma once

#include <lunaros/page.h>

mem_area_t *multiboot_parse_mmap(unsigned long addr, size_t *sz);
void multiboot_parse_info(unsigned long addr);
