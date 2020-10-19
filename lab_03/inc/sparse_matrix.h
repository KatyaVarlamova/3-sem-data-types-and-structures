#ifndef sparse_matrix_h
#define sparse_matrix_h

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include "vector.h"
#include "sparse_vector.h"
#include "ordinary_matrix.h"
#include <stdlib.h>
#define OK        0
#define ERR       1
#define INIT_SIZE 20
#define SC        "%ld"
#define PR        "%ld "
typedef struct
{
    vector_t data;
    vector_t rows;
    vector_t columns;
    size_t n;
    size_t m;
} sparse_matrix_t;
int alloc_sparse_matrix(sparse_matrix_t *matrix);
void free_sparse_matrix(sparse_matrix_t *matrix);
int read_all_sparse_matrix(FILE *f, sparse_matrix_t *matrix);
void print_sparse_matrix(sparse_matrix_t *matrix);
void multiply_vec_and_matrix(sparse_vector_t *vec, sparse_matrix_t *matrix, sparse_vector_t *res);
void zero_sparse_matrix(sparse_matrix_t *m);
int copy_sparse_matrix(ordinary_matrix_t *om, sparse_matrix_t *matrix);
#endif
