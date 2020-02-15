#include <std/string.h>
#include <std/stdbool.h>

#include <lunaros/buddy.h>
#include <lunaros/list.h>
#include <lunaros/page.h>
#include <lunaros/printf.h>

/*
** Physical page frame allocator.
** Uses the binary buddy algorithm. The maximum pages that can be allocated
** at once is stored in the struct. The minimum allocation request is a page.
**
** The buddy is design to have a low memory footprint, so it uses a bit binary
** tree. If the node is true it means it's splitted and the left node is free.
** A node in the false state means that it's either full or empty.
*/

static uint64_t log2(uint64_t n) {
   if (n == 0) return 0;
   return __builtin_ctzll(n);
}

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

void *buddy_tree_walk(buddy_area_t *b, size_t bkt) {
   list_t *p;
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
      p = node2ptr(b, node_left(node), walker);
      list_pushback(&b->freelist[walker], p);
      if (walker == bkt) { /* found the block requested? */
         char *ret = node2ptr(b, node_right(node), bkt);
         *(size_t *)ret = bkt;
         return ret + sizeof(size_t);
      }
      node = node_right(node);
      walker++;
   }
   return NULL;
}

void *buddy_alloc(buddy_area_t *b, size_t requested) {
   size_t node, bkt, realsz;
   void *p;
   bkt = buddy_find_bkt(b, requested + 8);
   realsz =  1 << (log2(b->max) - bkt);
   if (b->available - realsz > b->available)
      return NULL;
   b->available -= realsz;
   p = list_pop(&b->freelist[bkt]);
   if (!p) /* no bucket available */
      return buddy_tree_walk(b, bkt); /* try to split the memory blocks */
   node = ptr2node(b, (uintptr_t)p, bkt);
   if (node != 0) /* is node root? */
      node_parent_flip(b->tree, node);
   *(size_t *)p = bkt;
   return p + sizeof(size_t);
}

void buddy_free(buddy_area_t *b, void *ptr) {
   size_t node, sz;
   size_t *bkt;
   if (!ptr)
      return;
   bkt = (size_t *)((uintptr_t)ptr - sizeof(size_t)); /* allocation bucket */
   node = ptr2node(b, (uintptr_t)bkt, *bkt);
   while (node != 0) { /* while not root */
      /* this node is unused now */
      node_parent_flip(b->tree, node);
      if (node_is_split(b->tree, node_parent(node))) /* is our sibling used? */
         break;
      /* else remove sibling from free list */
      list_remove(node2ptr(b, node_sibling(node), *bkt));
      (*bkt)--;
      node = node_parent(node);
   }
   sz = 1 << (log2(b->max) - *bkt);
   b->available += sz;
   /* add the top most buddy to the free list */
   list_pushback(&b->freelist[*bkt], node2ptr(b, node, *bkt));
}

buddy_area_t *buddy_area_init(void *addr, size_t max, allocf_t alloc) {
   list_t *p = addr;
   buddy_area_t *b = alloc(sizeof(buddy_area_t));
   if (unlikely(b == NULL)) {
      pr_err("Not enough memory for buddy");
      return NULL;
   }
   memset(b, 0, sizeof(buddy_area_t));
   b->available = max;
   b->min = PGSIZE;
   b->max = max;
   b->base = addr;
   b->tree = alloc((1 << buddy_max_bucket(b)) / sizeof(b->tree));
   if (unlikely(b->tree == NULL)) {
      pr_err("Not enough memory for buddy");
      return NULL;
   }
   b->freelist = alloc(buddy_max_bucket(b) * sizeof(b->freelist));
   if (unlikely(b->freelist == NULL)) {
      pr_err("Not enough memory for buddy");
      return NULL;
   }
   list_init(&b->head);
   for (size_t i = 0; i < buddy_max_bucket(b); i++)
      list_init(&b->freelist[i]);
   list_pushback(&b->freelist[0], p);
   return b;
}
