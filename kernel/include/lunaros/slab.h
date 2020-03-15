#pragma once

#include <std/stdint.h>

#include <lunaros/list.h>

struct cache {
   size_t index;
   size_t entries_sz;
   void *entries[];
};

struct kmem_cache_node {
   size_t obj_size;        /* cache's object size */
   const char *name;       /* cache name */
   struct cache *cache;    /* the cache itself */
};

void *kmalloc(size_t size);
void kfree(void *p);
size_t ksize(void *p);

void init_slab(void);
