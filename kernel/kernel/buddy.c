#include <std/string.h>
#include <std/stdbool.h>

#include <lunaros/buddy.h>
#include <lunaros/kernel.h>
#include <lunaros/list.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>
#include <lunaros/vmm.h>

/*
** Physical page frame allocator.
** Uses the binary buddy algorithm. The maximum pages that can be allocated
** at once is stored in the struct. The minimum allocation request is a page.
**
** The buddy is design to have a low memory footprint, so it uses a bit binary
** tree. If the node is true it means it's splitted and the left node is free.
** A node in the false state means that it's either full or empty.
*/

static bool node_is_split(uint8_t *tree, size_t node) {
   return (tree[node / 8] >> (node % 8)) & 1;
}

static void node_set_split(uint8_t *tree, size_t node) {
   tree[node / 8] |= 1 << (node % 8);
}

static void node_parent_flip(uint8_t *tree, size_t node) {
   size_t parent = node_parent(node);
   tree[parent / 8] ^= 1 << (parent % 8);
}

static void *node2ptr(buddy_area_t *b, size_t node, size_t bkt) {
   uintptr_t base = (uintptr_t)b->base;
   return (void *)(base + ((node - (1 << bkt) + 1) << (log2(b->max) - bkt)));
}

static size_t ptr2node(buddy_area_t *b, uintptr_t page, size_t bkt) {
   uintptr_t base = (uintptr_t)b->base;
   return ((page - base) >> (log2(b->max) - bkt)) + (1 << bkt) - 1;
}

static size_t buddy_max_bucket(buddy_area_t *b) {
   return log2(b->max) - log2(b->min) + 1;
}

static size_t buddy_find_bkt(buddy_area_t *b, size_t requested) {
   size_t bkt = buddy_max_bucket(b);
   size_t sz = b->min;
   while (sz < requested) {
      bkt--;
      sz <<= 1;
   }
   return bkt - 1; /* actually an index */
}

void buddy_pushback(list_t *freelist, void *addr) {
   list_t *ph = vmm_phys2virt(addr);
   list_pushback(freelist, ph);
}

void *buddy_pop(list_t *freelist) {
   list_t *entry = list_pop(freelist);
   if (entry == NULL)
      return NULL;
   memset(entry, 0, sizeof(list_t));
   return (void *)entry;
}

void buddy_remove(void *addr) {
   list_t *ph = vmm_phys2virt(addr);
   list_remove(ph);
   memset(ph, 0, sizeof(list_t));
}

void *buddy_tree_walk(buddy_area_t *b, size_t bkt) {
   void *addr;
   size_t node = 0; /* root node */
   size_t walker = 1; /* child's bucket */
   while (walker <= bkt) {
      if (node_is_split(b->tree, node)) { /* right child is full */
         node = node_left(node);
         walker++;
         continue;
      }
      node_set_split(b->tree, node);
      /* insert the left child to the free list */
      addr = node2ptr(b, node_left(node), walker);
      buddy_pushback(&b->freelist[walker], addr);
      if (walker == bkt) {  /* found the block requested? */
         void *ret = node2ptr(b, node_right(node), bkt);
         struct page *page = phys2page(ret);
         page->order = bkt;
         return ret;
      }
      node = node_right(node);
      walker++;
   }
   return NULL;
}

void *buddy_alloc(buddy_area_t *b, size_t requested) {
   size_t node, bkt, realsz;
   struct page *page;
   void *ptr;
   bkt = buddy_find_bkt(b, requested);
   realsz =  1 << (log2(b->max) - bkt);
   if (b->available - realsz > b->available)
      return NULL;
   b->available -= realsz;
   ptr = buddy_pop(&b->freelist[bkt]);
   if (ptr == NULL) /* found available page? */
      return buddy_tree_walk(b, bkt); /* try to split the page blocks */
   node = ptr2node(b, (uintptr_t)ptr, bkt);
   if (node != 0) /* is node root? */
      node_parent_flip(b->tree, node);
   page = phys2page(ptr);
   page->order = bkt;
   return ptr;
}

void buddy_free(buddy_area_t *b, void *ptr) {
   struct page *page;
   size_t node, sz, bkt;
   if (!ptr)
      return;
   page = phys2page(ptr);
   if (page == NULL)
      return;
   bkt = (size_t)page->order;
   node = ptr2node(b, (uintptr_t)ptr, bkt);
   while (node != 0) { /* while not root */
      /* this node is unused now */
      node_parent_flip(b->tree, node);
      if (node_is_split(b->tree, node_parent(node))) /* is our sibling used? */
         break;
      /* else remove sibling from free list */
      buddy_remove(node2ptr(b, node_sibling(node), bkt));
      bkt--;
      node = node_parent(node);
   }
   sz = 1 << (log2(b->max) - bkt);
   b->available += sz;
   /* add the top most buddy to the free list */
   buddy_pushback(&b->freelist[bkt], node2ptr(b, node, bkt));
   memset(page, 0, sizeof(struct page));
}

buddy_area_t *buddy_area_init(void *addr, size_t max, allocf_t alloc) {
   buddy_area_t *b = alloc(sizeof(buddy_area_t));
   if (unlikely(b == NULL)) {
      pr_err("Not enough memory for buddy");
      return NULL;
   }
   b->available = max;
   b->min = PGSIZE;
   b->max = max;
   b->base = addr;
   b->tree = alloc((1 << buddy_max_bucket(b)) / sizeof(uint8_t));
   if (unlikely(b->tree == NULL)) {
      pr_err("Not enough memory for buddy");
      return NULL;
   }
   b->freelist = alloc(buddy_max_bucket(b) * sizeof(list_t));
   if (unlikely(b->freelist == NULL)) {
      pr_err("Not enough memory for buddy");
      return NULL;
   }
   list_init(&b->head);
   for (size_t i = 0; i < buddy_max_bucket(b); i++)
      list_init(&b->freelist[i]);
   buddy_pushback(&b->freelist[0], addr);
   return b;
}
