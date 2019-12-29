#pragma once

#include <std/stdint.h>

#include <lunaros/page.h>

void *mmap_physical_page(void *page, ppt_t type);
