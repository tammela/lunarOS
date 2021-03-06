#pragma once

#include <std/stdbool.h>
#include <std/stdint.h>

typedef struct list_t list_t;

struct list_t {
   struct list_t *prev;
   struct list_t *next;
};

void list_init(list_t *lst);
void list_pushback(list_t *lst, list_t *entry);
void list_remove(list_t *lst);
list_t *list_pop(list_t *lst);

bool list_empty(list_t *lst);

#define LIST_INIT(lst) { &(lst), &(lst) }

#define LIST_HEAD(lst) \
   list_t lst = LIST_INIT(lst);

#define list_for_each(cursor, head) \
   for(cursor = (head)->next; cursor != (head); cursor = cursor->next)
