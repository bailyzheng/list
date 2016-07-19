
//
// dl_list.h
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// Library version

#define LIST_VERSION "0.0.5"

// Memory management macros

#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif

#ifndef LIST_FREE
#define LIST_FREE free
#endif

/*
 * dl_list_t iterator direction.
 */

typedef enum {
    LIST_HEAD
  , LIST_TAIL
} dl_list_direction_t;

/*
 * dl_list_t node struct.
 */

typedef struct dl_list_node {
  struct dl_list_node *prev;
  struct dl_list_node *next;
  void *val;
} dl_list_node_t;

/*
 * dl_list_t struct.
 */

typedef struct {
  dl_list_node_t *head;
  dl_list_node_t *tail;
  unsigned int len;
  void (*free)(void *val);
  int (*match)(void *a, void *b);
} dl_list_t;

/*
 * dl_list_t iterator struct.
 */

typedef struct {
  dl_list_node_t *next;
  dl_list_direction_t direction;
} dl_list_iterator_t;

// Node prototypes.

dl_list_node_t *
dl_list_node_new(void *val);

// dl_list_t prototypes.

dl_list_t *
dl_list_new();

dl_list_node_t *
dl_list_rpush(dl_list_t *self, dl_list_node_t *node);

dl_list_node_t *
dl_list_lpush(dl_list_t *self, dl_list_node_t *node);

dl_list_node_t *
dl_list_find(dl_list_t *self, void *val);

dl_list_node_t *
dl_list_at(dl_list_t *self, int index);

dl_list_node_t *
dl_list_rpop(dl_list_t *self);

dl_list_node_t *
dl_list_lpop(dl_list_t *self);

void
dl_list_remove(dl_list_t *self, dl_list_node_t *node);

void
dl_list_destroy(dl_list_t *self);

// dl_list_t iterator prototypes.

dl_list_iterator_t *
dl_list_iterator_new(dl_list_t *list, dl_list_direction_t direction);

dl_list_iterator_t *
dl_list_iterator_new_from_node(dl_list_node_t *node, dl_list_direction_t direction);

dl_list_node_t *
dl_list_iterator_next(dl_list_iterator_t *self);

void
dl_list_iterator_destroy(dl_list_iterator_t *self);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */
