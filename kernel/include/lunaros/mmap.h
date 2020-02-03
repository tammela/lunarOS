#pragma once

#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/page.h>

void *mmap_physical_page(void *page, ppt_t type);
void *mmap_physical_area(void *start, size_t npages, ppt_t type);
