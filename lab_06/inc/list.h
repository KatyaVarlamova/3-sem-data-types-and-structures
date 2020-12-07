#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_ERROR 1
#define ACCESS_ERROR 2
#define OK 0

typedef struct node node_t;
struct node
{
    void *data;
    node_t *next;
};

node_t *find(node_t *head, const void *data, int (*comparator)(const void *, const void *), int *comp_count);

void * find_and_delete(node_t **head, const void *value, int (*comparator)(const void *, const void *), int *comp_count);

int push_front(node_t **head, node_t *elem);

void *pop_front(node_t **head);

int push_back(node_t **head, node_t *elem);

void *pop_back(node_t **head);

void print_list(FILE *f, node_t *head, void (*print_element)(FILE *f, const void *));

node_t *read_list(FILE *f, void *(*reader)(FILE *));

node_t *reverse(node_t *head);

void insert(node_t **head, node_t *elem, node_t *before);

void free_list(node_t **head, void (*free_object)(void *));

void front_back_split(node_t *head, node_t **back);

node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*comparator)(const void *, const void *));

node_t *sort(node_t *head, int (*comparator)(const void *, const void *));

node_t *create_node(void *data);

node_t *copy(node_t *head);

void *delete_node(node_t **head, node_t **elem);

#endif
