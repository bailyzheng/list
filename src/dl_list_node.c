
//
// node.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "dl_list.h"

/*
 * Allocates a new dl_list_node_t. NULL on failure.
 */

dl_list_node_t *
dl_list_node_new(void *val) {
  dl_list_node_t *self;
  if (!(self = LIST_MALLOC(sizeof(dl_list_node_t))))
    return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val;
  return self;
}
