#pragma once

#include <std/stddef.h>

#include <lunaros/list.h>
#include <lunaros/page.h>

typedef void *(*allocf_t)(size_t);

typedef struct buddy_area_t buddy_area_t;

struct buddy_area_t {
   void *base;          /* base address */
   uint8_t *tree;       /* bitwise binary tree */
   list_t *freelist;    /* buddy's freelist */
   uint64_t min;        /* minimal allocation size */
   uint64_t max;        /* maximum allocation size */
   list_t head;         /* buddies list head */
};

/* Tree walking macros */
#define node_parent(i)          ((i - 1) / 2)
#define node_left(i)            ((i * 2) + 1)
#define node_right(i)           ((i * 2) + 2)
#define node_sibling(i)         (((i - 1) ^ 1) + 1)

buddy_area_t *buddy_area_init(void *addr, size_t max, allocf_t alloc);
void *buddy_alloc(buddy_area_t *b, size_t requested);
void buddy_free(buddy_area_t *b, void *ptr);
