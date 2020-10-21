#ifndef vector_h
#define vector_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#define MEMORY_ERROR 1
#define OK           0
#define MAX          10

typedef struct
{
    void *front;
    size_t size_of_element;
    size_t size;
    size_t alloc_size;
} vector_t;

int alloc_vector(vector_t *v, size_t size_of_element, size_t size);

void free_vector(vector_t *v);

int push_back_to_vector(vector_t *v, void *elem);

void zero_vector(vector_t *v);

void pop_from_vector(vector_t *v);

void print_vector(vector_t *v);

void print_vector_st(vector_t *v);
#endif
