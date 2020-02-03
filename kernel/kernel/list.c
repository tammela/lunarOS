#include <std/stddef.h>

#include <lunaros/list.h>

void list_init(list_t *lst) {
   lst->prev = lst;
   lst->next = lst;
}

void list_insert(list_t *lst, list_t *entry) {
   list_t *prev = lst->prev;
   entry->prev = prev;
   entry->next = prev->next;
   prev->next = entry;
   prev->next->prev = entry;
}

void list_remove(list_t *entry) {
   list_t *prev = entry->prev;
   list_t *next = entry->next;
   prev->next = next;
   next->prev = prev;
}

list_t *list_pop(list_t *lst) {
   if (list_empty(lst))
      return NULL;
   list_t *back = lst->prev;
   list_remove(back);
   return back;
}

uint32_t list_empty(list_t *lst) {
   return lst->prev == lst;
}
