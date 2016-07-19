
//
// dl_list.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "dl_list.h"

/*
 * Allocate a new dl_list_t. NULL on failure.
 */

dl_list_t *
dl_list_new() {
  dl_list_t *self;
  if (!(self = LIST_MALLOC(sizeof(dl_list_t))))
    return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->free = NULL;
  self->match = NULL;
  self->len = 0;
  return self;
}

/*
 * Free the dl_list.
 */

void
dl_list_destroy(dl_list_t *self) {
  unsigned int len = self->len;
  dl_list_node_t *next;
  dl_list_node_t *curr = self->head;

  while (len--) {
    next = curr->next;
    if (self->free) self->free(curr->val);
    LIST_FREE(curr);
    curr = next;
  }

  LIST_FREE(self);
}

/*
 * Append the given node to the dl_list
 * and return the node, NULL on failure.
 */

dl_list_node_t *
dl_list_rpush(dl_list_t *self, dl_list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return / detach the last node in the dl_list, or NULL.
 */

dl_list_node_t *
dl_list_rpop(dl_list_t *self) {
  if (!self->len) return NULL;

  dl_list_node_t *node = self->tail;

  if (--self->len) {
    (self->tail = node->prev)->next = NULL;
  } else {
    self->tail = self->head = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Return / detach the first node in the dl_list, or NULL.
 */

dl_list_node_t *
dl_list_lpop(dl_list_t *self) {
  if (!self->len) return NULL;

  dl_list_node_t *node = self->head;

  if (--self->len) {
    (self->head = node->next)->prev = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Prepend the given node to the dl_list
 * and return the node, NULL on failure.
 */

dl_list_node_t *
dl_list_lpush(dl_list_t *self, dl_list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->next = self->head;
    node->prev = NULL;
    self->head->prev = node;
    self->head = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return the node associated to val or NULL.
 */

dl_list_node_t *
dl_list_find(dl_list_t *self, void *val) {
  dl_list_iterator_t *it = dl_list_iterator_new(self, LIST_HEAD);
  dl_list_node_t *node;

  while ((node = dl_list_iterator_next(it))) {
    if (self->match) {
      if (self->match(val, node->val)) {
        dl_list_iterator_destroy(it);
        return node;
      }
    } else {
      if (val == node->val) {
        dl_list_iterator_destroy(it);
        return node;
      }
    }
  }

  dl_list_iterator_destroy(it);
  return NULL;
}

/*
 * Return the node at the given index or NULL.
 */

dl_list_node_t *
dl_list_at(dl_list_t *self, int index) {
  dl_list_direction_t direction = LIST_HEAD;

  if (index < 0) {
    direction = LIST_TAIL;
    index = ~index;
  }

  if ((unsigned)index < self->len) {
    dl_list_iterator_t *it = dl_list_iterator_new(self, direction);
    dl_list_node_t *node = dl_list_iterator_next(it);
    while (index--) node = dl_list_iterator_next(it);
    dl_list_iterator_destroy(it);
    return node;
  }

  return NULL;
}

/*
 * Remove the given node from the dl_list, freeing it and it's value.
 */

void
dl_list_remove(dl_list_t *self, dl_list_node_t *node) {
  node->prev
    ? (node->prev->next = node->next)
    : (self->head = node->next);

  node->next
    ? (node->next->prev = node->prev)
    : (self->tail = node->prev);

  if (self->free) self->free(node->val);

  LIST_FREE(node);
  --self->len;
}
