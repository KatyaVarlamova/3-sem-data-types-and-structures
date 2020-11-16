#ifndef queue_on_list_h
#define queue_on_list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "vector.h"

#define MEMORY_ERROR 1
#define LENGTH_ERROR 2
#define OK           0
#define INIT_SIZE    4

typedef struct node
{
    struct node *next;
    elem_t *value;
} node_t;

typedef struct
{
    node_t *head;
    node_t *tail;
    vector_t adrs;
    size_t count;
} queue_on_list_t;

void init_queue_on_list(queue_on_list_t *queue);

size_t get_length_in_list(void *q);

void delete_queue_on_list(queue_on_list_t *queue, void (* free_elem)(elem_t *));

int read_queue_on_list(FILE *f, queue_on_list_t *queue, elem_t * (* reader)(FILE *));

void print_queue_on_list(FILE *f, void *q, void (* printer)(FILE *, const elem_t *));

int push_to_queue_on_list(void *q, void *value);

void * pop_from_queue_on_list(void *q);

void * get_front_in_queue_on_list(void *q);

int insert_to_queue_on_list(void *q, void *value);

#endif
