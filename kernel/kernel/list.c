#include <std/stdbool.h>
#include <std/stddef.h>

#include <lunaros/list.h>

void list_init(list_t *lst) {
   lst->prev = lst;
   lst->next = lst;
}

void list_pushback(list_t *lst, list_t *entry) {
   list_t *prev = lst->prev;
   prev->next = entry;
   lst->prev = entry;
   entry->prev = prev;
   entry->next = lst;
}

void list_remove(list_t *entry) {
   list_t *prev = entry->prev;
   list_t *next = entry->next;
   prev->next = next;
   next->prev = prev;
   entry->next = entry;
   entry->prev = entry;
}

list_t *list_pop(list_t *lst) {
   if (list_empty(lst))
      return NULL;
   list_t *back = lst->prev;
   list_remove(back);
   return back;
}

bool list_empty(list_t *lst) {
   return lst->prev == lst;
}
