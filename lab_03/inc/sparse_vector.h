#ifndef sparse_vector_h
#define sparse_vector_h

#include <stdio.h>
#include <assert.h>
#include "vector.h"
#include <stdlib.h>
#define OK        0
#define ERR       1
#define INIT_SIZE 20
#define SC        "%ld"
#define PR        "%ld "
typedef struct
{
    vector_t data;
    vector_t columns;
    size_t n;
} sparse_vector_t;
int alloc_sparse_vector(sparse_vector_t *vec);
void free_sparse_vector(sparse_vector_t *vec);
int read_all_sparse_vector(FILE *f, sparse_vector_t *vec);
void print_sparse_vector(sparse_vector_t *vec);
void zero_sparse_vector(sparse_vector_t *v);
int copy_sparse_vector(vector_t *ov, sparse_vector_t *vec);
#endif
