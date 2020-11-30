#ifndef balanced_tree_h
#define balanced_tree_h

#include <stdio.h>
#include <stdlib.h>
#include "element.h"

#define MEMORY_ERROR 1
#define LENGTH_ERROR 2
#define OK 0

typedef struct BNode {
    elem_t *data;
    struct BNode *left;
    struct BNode *right;
    short height;
} balanced_tree_node_t;

balanced_tree_node_t * create_balanced_tree_node(elem_t *elem);

int count_elems_btree(balanced_tree_node_t *btree);

void delete_balanced_tree_node(balanced_tree_node_t **node, void (* free_elem)(void *val));

balanced_tree_node_t * remove_balanced_tree_node(balanced_tree_node_t *tree, const balanced_tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), void (* free_elem)(void *val), stat_values_t *stat);

balanced_tree_node_t * insert_balanced_tree_node(balanced_tree_node_t *tree, balanced_tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), int *added);

balanced_tree_node_t * read_balanced_tree(char *filename, void *(*reader)(FILE *f), int (* comp)(const void *lhs, const void *rhs));

void print_balanced_tree(FILE *f, balanced_tree_node_t *tree, void print_elem(FILE *f, const void *elem));

void free_balanced_tree(balanced_tree_node_t *tree, void (* free_elem)(void *val));

void btree_to_dot(char *name,balanced_tree_node_t *tree, void print_elem(FILE *f, const void *elem));

#endif 
