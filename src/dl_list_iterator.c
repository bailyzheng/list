
//
// iterator.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "dl_list.h"

/*
 * Allocate a new dl_list_iterator_t. NULL on failure.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */

dl_list_iterator_t *
dl_list_iterator_new(dl_list_t *list, dl_list_direction_t direction) {
  dl_list_node_t *node = direction == LIST_HEAD
    ? list->head
    : list->tail;
  return dl_list_iterator_new_from_node(node, direction);
}

/*
 * Allocate a new dl_list_iterator_t with the given start
 * node. NULL on failure.
 */

dl_list_iterator_t *
dl_list_iterator_new_from_node(dl_list_node_t *node, dl_list_direction_t direction) {
  dl_list_iterator_t *self;
  if (!(self = LIST_MALLOC(sizeof(dl_list_iterator_t))))
    return NULL;
  self->next = node;
  self->direction = direction;
  return self;
}

/*
 * Return the next dl_list_node_t or NULL when no more
 * nodes remain in the list.
 */

dl_list_node_t *
dl_list_iterator_next(dl_list_iterator_t *self) {
  dl_list_node_t *curr = self->next;
  if (curr) {
    self->next = self->direction == LIST_HEAD
      ? curr->next
      : curr->prev;
  }
  return curr;
}

/*
 * Free the list iterator.
 */

void
dl_list_iterator_destroy(dl_list_iterator_t *self) {
  LIST_FREE(self);
  self = NULL;
}
