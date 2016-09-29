#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rbtree.h"
#include "rbmap.h"


RBMapNode *
rbmap_new_node(RBMapKey key, void *value) {
  RBMapNode *node = (RBMapNode *)malloc(sizeof(RBMapNode));
  node->key = key;
  node->value = value;
  return node;
}


void
rbmap_free_node(RBMapNode *map_node) {
  if(map_node == NULL) {
    return;
  }

  map_node->value = NULL;
  free(map_node);
  return;
}


RBMap *
rbmap_new() {
  RBMap *map = (RBMap *)malloc(sizeof(RBMap));
  map->tree = (struct rb_root *)malloc(sizeof(struct rb_root));
  map->tree->rb_node = NULL;
  map->cnt = 0;
  return map;
}

void
rbmap_free(RBMap *map) {
  struct rb_root *rb_tree = map->tree;

  /* // del by root */
  /* struct rb_node *rb_node = rb_tree->rb_node; */
  /* RBMapNode *rbmap_node = NULL; */
  /* while(rb_node) { */
  /*   rb_erase(rb_node, rb_tree); */
  /*   rbmap_node = rb_entry(rb_node, RBMapNode, node); */
  /*   rbmap_free_node(rbmap_node); */
  /*   rb_node = rb_tree->rb_node; */
  /* } */

  // del by first
  struct rb_node *rb_node = rb_first(rb_tree);
  RBMapNode *rbmap_node = NULL;
  while(rb_node) {
    rb_erase(rb_node, rb_tree);
    rbmap_node = rb_entry(rb_node, RBMapNode, node);
    rbmap_free_node(rbmap_node);
    rb_node = rb_first(rb_tree);
  }

  free(map->tree);
  map->tree = NULL;
  free(map);
  return;
}


RBMapNode *
rbmap_get(RBMap *map, RBMapKey key) {
  struct rb_node *rb_node = map->tree->rb_node;
  RBMapNode *rbmap_node = NULL;
  while(rb_node) {
    rbmap_node = rb_entry(rb_node, RBMapNode, node);
    int result = RBMapKeyCmp(key, rbmap_node->key);
    if (result < 0) {
      rb_node = rb_node->rb_left;

    } else if (result > 0) {
      rb_node = rb_node->rb_right;

    } else {
      return rbmap_node;
    }
  }
  return NULL;
}


int 
rbmap_insert(RBMap *map, RBMapKey key, void *value) {
  struct rb_node **new = &(map->tree->rb_node), *parent = NULL;
  RBMapNode *this = NULL;
  /* Figure out where to put new node */
  while(*new) {
    this = rb_entry(*new, RBMapNode, node);
    int result = RBMapKeyCmp(key, this->key);

    parent = *new;
    if (result < 0) {
      new = &((*new)->rb_left);

    } else if (result > 0) {
      new = &((*new)->rb_right);

    } else {
      return 0;
    }
  }

  RBMapNode *map_node = rbmap_new_node(key, value);
  /* Add new node and rebalance tree. */
  rb_link_node(&map_node->node, parent, new);
  rb_insert_color(&map_node->node, map->tree);
  map->cnt = map->cnt + 1;
  return 1;
}


void
rbmap_upsert(RBMap *map, RBMapKey key, void *value) {
  struct rb_node **new = &(map->tree->rb_node), *parent = NULL;
  RBMapNode *this = NULL;
  RBMapNode *map_node = NULL;
  /* Figure out where to put new node */
  while(*new) {
    this = rb_entry(*new, RBMapNode, node);
    int result = RBMapKeyCmp(key, this->key);

    parent = *new;
    if (result < 0) {
      new = &((*new)->rb_left);

    } else if (result > 0) {
      new = &((*new)->rb_right);

    } else {
      map_node = this;
      break;
    }
  }

  if(map_node) {
    map_node->value = value;
  } else {
    RBMapNode *map_node = rbmap_new_node(key, value);
    /* Add new node and rebalance tree. */
    rb_link_node(&map_node->node, parent, new);
    rb_insert_color(&map_node->node, map->tree);
    map->cnt = map->cnt + 1;
  }
  return;
}


void
rbmap_remove(RBMap *map, RBMapKey key) {
  RBMapNode *rbmap_node = rbmap_get(map, key);
  if (!rbmap_node) {
    return;
  }

  rb_erase(&rbmap_node->node, map->tree);
  rbmap_free_node(rbmap_node);
  map->cnt = map->cnt - 1;
  return;
}

