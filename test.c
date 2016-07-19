
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "src/dl_list.h"

// Helpers

#define test(fn) \
  puts("... \x1b[33m" # fn "\x1b[0m"); \
  test_##fn();

static int freeProxyCalls = 0;

void
freeProxy(void *val) {
  ++freeProxyCalls;
  free(val);
}

typedef struct {
  char *name;
} User;

static int
User_equal(User *a, User *b) {
  return 0 == strcmp(a->name, b->name);
}

// Tests

static void
test_dl_list_node_new() {
  char *val = "some value";
  dl_list_node_t *node = dl_list_node_new(val);
  assert(node->val == val);
  free(node);
}

static void
test_dl_list_rpush() {
  // Setup
  dl_list_t *list = dl_list_new();
  dl_list_node_t *a = dl_list_node_new("a");
  dl_list_node_t *b = dl_list_node_new("b");
  dl_list_node_t *c = dl_list_node_new("c");

  // a b c
  dl_list_rpush(list, a);
  dl_list_rpush(list, b);
  dl_list_rpush(list, c);

  // Assertions
  assert(a == list->head);
  assert(c == list->tail);
  assert(3 == list->len);
  assert(b == a->next);
  assert(NULL == a->prev);
  assert(c == b->next);
  assert(a == b->prev);
  assert(NULL == c->next);
  assert(b == c->prev);

  dl_list_destroy(list);
}

static void
test_dl_list_lpush() {
  // Setup
  dl_list_t *list = dl_list_new();
  dl_list_node_t *a = dl_list_node_new("a");
  dl_list_node_t *b = dl_list_node_new("b");
  dl_list_node_t *c = dl_list_node_new("c");

  // c b a
  dl_list_rpush(list, a);
  dl_list_lpush(list, b);
  dl_list_lpush(list, c);

  // Assertions
  assert(c == list->head);
  assert(a == list->tail);
  assert(3 == list->len);
  assert(NULL == a->next);
  assert(b == a->prev);
  assert(a == b->next);
  assert(c == b->prev);
  assert(b == c->next);
  assert(NULL == c->prev);

  dl_list_destroy(list);
}

static void
test_dl_list_at() {
  // Setup
  dl_list_t *list = dl_list_new();
  dl_list_node_t *a = dl_list_node_new("a");
  dl_list_node_t *b = dl_list_node_new("b");
  dl_list_node_t *c = dl_list_node_new("c");

  // a b c
  dl_list_rpush(list, a);
  dl_list_rpush(list, b);
  dl_list_rpush(list, c);

  // Assertions
  assert(a == dl_list_at(list, 0));
  assert(b == dl_list_at(list, 1));
  assert(c == dl_list_at(list, 2));
  assert(NULL == dl_list_at(list, 3));

  assert(c == dl_list_at(list, -1));
  assert(b == dl_list_at(list, -2));
  assert(a == dl_list_at(list, -3));
  assert(NULL == dl_list_at(list, -4));

  dl_list_destroy(list);
}

static void
test_dl_list_destroy() {
  // Setup
  dl_list_t *a = dl_list_new();
  dl_list_destroy(a);

  // a b c
  dl_list_t *b = dl_list_new();
  dl_list_rpush(b, dl_list_node_new("a"));
  dl_list_rpush(b, dl_list_node_new("b"));
  dl_list_rpush(b, dl_list_node_new("c"));
  dl_list_destroy(b);

  // Assertions
  dl_list_t *c = dl_list_new();
  c->free = freeProxy;
  dl_list_rpush(c, dl_list_node_new(dl_list_node_new("a")));
  dl_list_rpush(c, dl_list_node_new(dl_list_node_new("b")));
  dl_list_rpush(c, dl_list_node_new(dl_list_node_new("c")));
  dl_list_destroy(c);
  assert(3 == freeProxyCalls);
}

static void
test_dl_list_find() {
  // Setup
  dl_list_t *langs = dl_list_new();
  dl_list_node_t *js = dl_list_rpush(langs, dl_list_node_new("js"));
  dl_list_node_t *ruby = dl_list_rpush(langs, dl_list_node_new("ruby"));

  dl_list_t *users = dl_list_new();
  users->match = User_equal;
  User userTJ = { "tj" };
  User userSimon = { "simon" };
  User userTaylor = { "taylor" };
  dl_list_node_t *tj = dl_list_rpush(users, dl_list_node_new(&userTJ));
  dl_list_node_t *simon = dl_list_rpush(users, dl_list_node_new(&userSimon));

  // Assertions
  dl_list_node_t *a = dl_list_find(langs, "js");
  dl_list_node_t *b = dl_list_find(langs, "ruby");
  dl_list_node_t *c = dl_list_find(langs, "foo");
  assert(js == a);
  assert(ruby == b);
  assert(NULL == c);

  dl_list_destroy(langs);

  a = dl_list_find(users, &userTJ);
  b = dl_list_find(users, &userSimon);
  c = dl_list_find(users, &userTaylor);
  assert(tj == a);
  assert(simon == b);
  assert(NULL == c);

  dl_list_destroy(users);
}

static void
test_dl_list_remove() {
  // Setup
  dl_list_t *dl_list = dl_list_new();
  dl_list_node_t *a = dl_list_rpush(dl_list, dl_list_node_new("a"));
  dl_list_node_t *b = dl_list_rpush(dl_list, dl_list_node_new("b"));
  dl_list_node_t *c = dl_list_rpush(dl_list, dl_list_node_new("c"));

  // Assertions
  assert(3 == dl_list->len);

  dl_list_remove(dl_list, b);
  assert(2 == dl_list->len);
  assert(a == dl_list->head);
  assert(c == dl_list->tail);
  assert(c == a->next);
  assert(NULL == a->prev);
  assert(NULL == c->next);
  assert(a == c->prev);

  dl_list_remove(dl_list, a);
  assert(1 == dl_list->len);
  assert(c == dl_list->head);
  assert(c == dl_list->tail);
  assert(NULL == c->next);
  assert(NULL == c->prev);

  dl_list_remove(dl_list, c);
  assert(0 == dl_list->len);
  assert(NULL == dl_list->head);
  assert(NULL == dl_list->tail);

  dl_list_destroy(dl_list);
}

static void
test_dl_list_rpop() {
  // Setup
  dl_list_t *dl_list = dl_list_new();
  dl_list_node_t *a = dl_list_rpush(dl_list, dl_list_node_new("a"));
  dl_list_node_t *b = dl_list_rpush(dl_list, dl_list_node_new("b"));
  dl_list_node_t *c = dl_list_rpush(dl_list, dl_list_node_new("c"));

  // Assertions
  assert(3 == dl_list->len);

  assert(c == dl_list_rpop(dl_list));
  assert(2 == dl_list->len);
  assert(a == dl_list->head);
  assert(b == dl_list->tail);
  assert(a == b->prev);
  assert(NULL == dl_list->tail->next && "new tail node next is not NULL");
  assert(NULL == c->prev && "detached node prev is not NULL");
  assert(NULL == c->next && "detached node next is not NULL");

  free(c);

  assert(b == dl_list_rpop(dl_list));
  assert(1 == dl_list->len);
  assert(a == dl_list->head);
  assert(a == dl_list->tail);

  free(b);

  assert(a == dl_list_rpop(dl_list));
  assert(0 == dl_list->len);
  assert(NULL == dl_list->head);
  assert(NULL == dl_list->tail);

  free(a);

  assert(NULL == dl_list_rpop(dl_list));
  assert(0 == dl_list->len);

  dl_list_destroy(dl_list);
}

static void
test_dl_list_lpop() {
  // Setup
  dl_list_t *dl_list = dl_list_new();
  dl_list_node_t *a = dl_list_rpush(dl_list, dl_list_node_new("a"));
  dl_list_node_t *b = dl_list_rpush(dl_list, dl_list_node_new("b"));
  dl_list_node_t *c = dl_list_rpush(dl_list, dl_list_node_new("c"));

  // Assertions
  assert(3 == dl_list->len);

  assert(a == dl_list_lpop(dl_list));
  assert(2 == dl_list->len);
  assert(b == dl_list->head);
  assert(NULL == dl_list->head->prev && "new head node prev is not NULL");
  assert(NULL == a->prev && "detached node prev is not NULL");
  assert(NULL == a->next && "detached node next is not NULL");

  free(a);

  assert(b == dl_list_lpop(dl_list));
  assert(1 == dl_list->len);

  free(b);

  assert(c == dl_list_lpop(dl_list));
  assert(0 == dl_list->len);
  assert(NULL == dl_list->head);
  assert(NULL == dl_list->tail);

  free(c);

  assert(NULL == dl_list_lpop(dl_list));
  assert(0 == dl_list->len);

  dl_list_destroy(dl_list);
}

static void
test_dl_list_iterator_t() {
  // Setup
  dl_list_t *dl_list = dl_list_new();
  dl_list_node_t *tj = dl_list_node_new("tj");
  dl_list_node_t *taylor = dl_list_node_new("taylor");
  dl_list_node_t *simon = dl_list_node_new("simon");

  // tj taylor simon
  dl_list_rpush(dl_list, tj);
  dl_list_rpush(dl_list, taylor);
  dl_list_rpush(dl_list, simon);

  // Assertions

  // From head
  dl_list_iterator_t *it = dl_list_iterator_new(dl_list, LIST_HEAD);
  dl_list_node_t *a = dl_list_iterator_next(it);
  dl_list_node_t *b = dl_list_iterator_next(it);
  dl_list_node_t *c = dl_list_iterator_next(it);
  dl_list_node_t *d = dl_list_iterator_next(it);

  assert(a == tj);
  assert(b == taylor);
  assert(c == simon);
  assert(NULL == d);

  dl_list_iterator_destroy(it);

  // From tail
  it = dl_list_iterator_new(dl_list, LIST_TAIL);
  dl_list_node_t *a2 = dl_list_iterator_next(it);
  dl_list_node_t *b2 = dl_list_iterator_next(it);
  dl_list_node_t *c2 = dl_list_iterator_next(it);
  dl_list_node_t *d2 = dl_list_iterator_next(it);

  assert(a2 == simon);
  assert(b2 == taylor);
  assert(c2 == tj);
  assert(NULL == d2);
  dl_list_iterator_destroy(it);

  dl_list_destroy(dl_list);
}

int
main(void){
  printf("\ndl_list_t: %ld\n", sizeof(dl_list_t));
  printf("dl_list_node_t: %ld\n", sizeof(dl_list_node_t));
  printf("dl_list_iterator_t: %ld\n\n", sizeof(dl_list_iterator_t));
  test(dl_list_node_new);
  test(dl_list_rpush);
  test(dl_list_lpush);
  test(dl_list_find);
  test(dl_list_at);
  test(dl_list_remove);
  test(dl_list_rpop);
  test(dl_list_lpop);
  test(dl_list_destroy);
  test(dl_list_iterator_t);
  puts("... \x1b[32m100%\x1b[0m\n");
  return 0;
}
