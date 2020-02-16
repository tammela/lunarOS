#pragma once

#include <std/stdint.h>
#include <std/stddef.h>

#include <lunaros/pagetable.h>
#include <lunaros/page.h>

#define MM_PHYSMEM_LAYOUT_SZ    (8)

void *mm_reserved_alloc(size_t sz);

void mm_init(physmem_layout_t **layouts, size_t physoff, pte_t *reserved);

typedef enum {
   /*
   ** Allocate memory that eventually will be freed.
   */
   MM_RECLAIM = 0,
   /*
   ** Allocate memory that will definitely not be freed until it's powered off.
   ** We will rely on `mm_reserved_alloc()` calls.
   */
   MM_NORECLAIM = 1
} mm_flags_t;
