#ifndef ordinary_matrix_h
#define ordinary_matrix_h
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#define OK        0
#define ERR       1
#define MAX       10
#define SC        "%ld"
#define PR        "%ld "
typedef struct
{
    elem_t **ptrs;
    elem_t *data;
    size_t n, m;
} ordinary_matrix_t;
int alloc_ord_matrix(ordinary_matrix_t *matrix, size_t n, size_t m);
void free_ord_matrix(ordinary_matrix_t *matrix);
int read_all_matrix(FILE *f, ordinary_matrix_t *matrix);
int read_matrix_by_index(ordinary_matrix_t *matrix);
void print_matrix(ordinary_matrix_t *matrix);
void multiply_vec_and_ord_matrix(vector_t *vec, ordinary_matrix_t *matrix, vector_t *res);
int read_vector(FILE *f, vector_t *v);
void copy_matrix(ordinary_matrix_t *src, ordinary_matrix_t *dst);
void print_vector(vector_t *v);
void zero_ord_matrix(ordinary_matrix_t *m);
void fill_matrix_auto_with_percentage(ordinary_matrix_t *matrix, double percentage);
#endif 
