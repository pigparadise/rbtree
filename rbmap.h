#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rbtree.h"

// #define RBMAP_KEY_TYPE_NUM
#ifdef RBMAP_KEY_TYPE_NUM
  #define RBMapKey uint64_t
  #define RBMapKeyCmp(a, b) ((a < b) ? -1 : (a > b))
#else
  #define RBMapKey char *
  #define RBMapKeyCmp strcmp
#endif

typedef struct rbmap_node {
  struct rb_node node;
  RBMapKey key;
  void *value;
} RBMapNode;

typedef struct rbmap {
  struct rb_root *tree;
  int cnt;
} RBMap;

RBMapNode *
rbmap_new_node(RBMapKey key, void *value);

void
rbmap_free_node(RBMapNode *map_node);

RBMap *
rbmap_new();

void
rbmap_free(RBMap *map);

RBMapNode *
rbmap_get(RBMap *map, RBMapKey key);

int 
rbmap_insert(RBMap *map, RBMapKey key, void *value);

void
rbmap_upsert(RBMap *map, RBMapKey key, void *value);

void
rbmap_remove(RBMap *map, RBMapKey key);
