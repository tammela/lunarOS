#pragma once

#include <std/stdint.h>
#include <std/stddef.h>

#include <lunaros/pagetable.h>
#include <lunaros/page.h>

#define MM_PHYSMEM_LAYOUT_SZ    (8)

void mm_init(physmem_layout_t **layouts, size_t physoff, pde_t *reserved);
