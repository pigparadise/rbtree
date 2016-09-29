#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rbtree.h"
#include "rbmap.h"

int
main(int argc, char *args) {
  RBMap *map = rbmap_new();

  int idx;
  #ifdef RBMAP_KEY_TYPE_NUM
  RBMapKey keys[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  #else
  RBMapKey keys[] = {
    "a", "b", "c", "d", "e", 
    "f", "g", "h", "i", "j",
  };
  #endif

  char * values[] = {
    "a", "b", "c", "d", "e", 
    "f", "g", "h", "i", "j",
  };

  rbmap_insert(map, keys[2], values[2]);
  rbmap_insert(map, keys[5], values[5]);
  rbmap_insert(map, keys[1], values[1]);
  rbmap_insert(map, keys[3], values[3]);
  rbmap_insert(map, keys[8], values[8]);
  rbmap_insert(map, keys[0], values[0]);

  struct rb_node *node;
  printf("-- show.a, cnt:%d\n", map->cnt);
  RBMapNode *rbmap_node = NULL;
  for(node = rb_first(map->tree); node; node = rb_next(node)) {
    rbmap_node = rb_entry(node, RBMapNode, node);
#ifdef RBMAP_KEY_TYPE_NUM
    printf("key=%ld, value=\"%s\"\n", rbmap_node->key, (char*)rbmap_node->value);
#else
    printf("key=\"%s\", value=\"%s\"\n", rbmap_node->key, (char*)rbmap_node->value);
#endif
  }

  rbmap_upsert(map, keys[2], values[0]);
  rbmap_remove(map, keys[8]);
  rbmap_remove(map, keys[1]);
  rbmap_remove(map, keys[3]);

  printf("\n-- show.b, cnt:%d\n", map->cnt);
  for (node = rb_first(map->tree); node; node = rb_next(node)) {
    rbmap_node = rb_entry(node, RBMapNode, node);
#ifdef RBMAP_KEY_TYPE_NUM
    printf("key=%ld, value=\"%s\"\n", rbmap_node->key, (char*)rbmap_node->value);
#else
    printf("key=\"%s\", value=\"%s\"\n", rbmap_node->key, (char*)rbmap_node->value);
#endif
  }

  rbmap_free(map);
  return 0;
}

