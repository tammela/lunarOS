#include <lunaros/kernel.h>
#include <lunaros/page.h>
#include <lunaros/slab.h>
#include <lunaros/vmm.h>

void *cache_get(struct cache *cache) {
   if (cache->entries == NULL || cache->index > cache->entries_sz)
      return NULL;
   return cache->entries[cache->index++];
}

void cache_put(struct cache *cache, void *obj) {
   if (unlikely(cache->index == 0))
      panic("Spurious put in slab\n");
   cache->entries[--cache->index] = obj;
}

static struct kmem_cache_node slabs[] = {
   { 8, "kmem-00008", NULL },
   { 16, "kmem-00016", NULL },
   { 32, "kmem-00032", NULL },
   { 64, "kmem-00064", NULL },
   { 128, "kmem-00128", NULL },
   { 256, "kmem-00256", NULL },
   { 512, "kmem-00512", NULL },
   { 1024, "kmem-01024", NULL },
   { 0, NULL, NULL }
};

static size_t kmalloc_index(size_t size) {
   if (size <= 8)
      return 0;
   else if (size <= 16)
      return 1;
   else if (size <= 32)
      return 2;
   else if (size <= 64)
      return 3;
   else if (size <= 128)
      return 4;
   else if (size <= 256)
      return 5;
   else if (size <= 512)
      return 6;
   else if (size <= 1024)
      return 7;
   else
      return 8;
}

void *kmalloc(size_t size) {
   size_t index = kmalloc_index(size);
   return cache_get(slabs[index].cache);
}

void kfree(void *ptr) {
   struct kmem_cache_node *slab;
   if (ptr == NULL)
      return;
   slab = vmm_virt2slab(ptr);
   if (slab == NULL)
      return;
   cache_put(slab->cache, ptr);
}

size_t ksize(void *ptr) {
   struct kmem_cache_node *slab;
   if (ptr == NULL)
      return 0;
   slab = vmm_virt2slab(ptr);
   if (slab == NULL)
      return 0;
   return slab->obj_size;
}

static void alloc_cache(struct kmem_cache_node *node, size_t npages) {
   size_t entries_sz = (npages * PGSIZE) / node->obj_size;
   size_t alloc_sz = sizeof(struct cache) + entries_sz * sizeof(void *);
   size_t request = (alloc_sz / PGSIZE) + 1;
   struct cache *cache = page_alloc(request);
   if (cache == NULL)
      panic("Not enough memory for SLAB allocator!");
   node->cache = vmm_virt2phys(cache);
}

static void fill_cache(struct kmem_cache_node *node, void *p) {
   char *anchor = p;
   for (size_t i = 0; i < PGSIZE; i += node->obj_size)
      node->cache->entries[i / node->obj_size] = (void *)(anchor + i);
}

void init_slab(void) {
   for (size_t i = 0; i < (sizeof(slabs) / sizeof(slabs[0])); i++) {
      struct page *page;
      void *p;
      if (slabs[i].obj_size == 0)
         continue;
      alloc_cache(&slabs[i], 1);
      p = page_alloc(1);
      if (p == NULL)
         panic("Not enough memory for SLAB!");
      page = phys2page(p);
      page->slab = &slabs[i];
      fill_cache(&slabs[i], vmm_phys2virt(p));
   }
}
