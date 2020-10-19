#ifndef vector_h
#define vector_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MEMORY_ERROR 1
#define OK           0
#define MAX          10
#define SC        "%ld"
#define PR        "%ld "
typedef struct
{
    void *front;
    size_t size_of_element;
    size_t size;
    size_t alloc_size;
} vector_t;
typedef long elem_t;
int alloc_vector(vector_t *v, size_t size_of_element, size_t size);
void free_vector(vector_t *v);
int push_back_to_vector(vector_t *v, void *elem);
void * element_at(vector_t *v, size_t pos);
void zero_vector(vector_t *v);
void fill_vector_auto_with_percentage(vector_t *vec, double percentage);
void read_vector_by_index(vector_t *v);
#endif
