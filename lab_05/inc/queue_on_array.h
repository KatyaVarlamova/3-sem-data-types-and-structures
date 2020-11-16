#ifndef queue_on_array_h
#define queue_on_array_h

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "element.h"
#define MEMORY_ERROR 1
#define LENGTH_ERROR 2
#define OK 0
#define INIT_SIZE 4

typedef struct {
    elem_t **data;
    size_t head;
    size_t tail;
    size_t count;
    size_t max_count;
} queue_on_array_t;

int init_queue_on_array(queue_on_array_t *queue, size_t max_count);
size_t get_head(queue_on_array_t *queue);

void delete_queue_on_array(queue_on_array_t *queue, void (* free_elem)(elem_t *));

int read_queue_on_array(FILE *f, queue_on_array_t *queue, elem_t * (* reader)(FILE *));

void print_queue_on_array(FILE *f, void *q, void (* printer)(FILE *, const elem_t *));

int push_to_queue_on_array(void *q, void *value);

void * pop_from_queue_on_array(void *q);

void * get_front_in_queue_on_array(void *q);

int insert_to_queue_on_array(void *q, void *value);

size_t get_length_of_array(void *q);

#endif 
