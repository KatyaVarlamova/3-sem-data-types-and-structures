#ifndef tree_h
#define tree_h

#include <stdio.h>
#include <stdlib.h>
#include "element.h"

#define MEMORY_ERROR 1
#define LENGTH_ERROR 2
#define OK 0

typedef struct Node {
    elem_t *data;
    struct Node *left;
    struct Node *right;
} tree_node_t;
int count_elems_tree(tree_node_t *tree);

tree_node_t * create_tree_node(elem_t *elem);

void delete_tree_node(tree_node_t **node, void (* free_elem)(void *val));

tree_node_t * remove_tree_node(tree_node_t *tree, const tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), void (* free_elem)(void *val), stat_values_t *stat);

tree_node_t * insert_tree_node(tree_node_t *tree, tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), int *added);

tree_node_t * read_tree(char *filename, void *(*reader)(FILE *f), int (* comp)(const void *lhs, const void *rhs));

void print_tree(FILE *f, tree_node_t *tree, void print_elem(FILE *f, const void *elem));

void free_tree(tree_node_t *tree, void (* free_elem)(void *val));

void tree_to_dot(char *name, tree_node_t *tree, void print_elem(FILE *f, const void *elem));

#endif 
