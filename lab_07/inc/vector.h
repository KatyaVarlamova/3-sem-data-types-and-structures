#ifndef vector_h
#define vector_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"

#define OK           0
#define MEMORY_ERROR 1

typedef struct
{
    elem_t **front;
    size_t size;
    size_t alloc_size;
} vector_t;

vector_t * alloc_vector(size_t size);

void free_vector(vector_t *v, void (* free_elem)(void *val));

int push_back_to_vector(vector_t *v, elem_t *elem);

void read_vector(FILE *f, vector_t *vector, void *(*reader)(FILE *f, size_t max), size_t max);

void delete_elem_in_vector(vector_t *vector, elem_t *value, int (* comp)(const void *lhs, const void *rhs), void (* free_elem)(void *val));

void print_vector(FILE *f, vector_t *vector, void print_elem(FILE *f, const void *elem));

int resize(vector_t *v, size_t new_size);

void to_dot(char *name, vector_t *vec);

#endif
