#include "ordinary_matrix.h"
int alloc_ord_matrix(ordinary_matrix_t *matrix, size_t n, size_t m)
{
    if (n == 0 || m == 0)
        return ERR;
    matrix->ptrs = calloc(n, sizeof(elem_t *));
    if (matrix->ptrs == NULL)
        return ERR;
    matrix->data = calloc(n * m, sizeof(elem_t));
    if (matrix->data == NULL)
    {
        free(matrix->ptrs);
        return ERR;
    }
    for (size_t i = 0; i < n; i++)
        matrix->ptrs[i] = matrix->data + i * m;
    return OK;
}
void free_ord_matrix(ordinary_matrix_t *matrix)
{
    free(matrix->ptrs);
    free(matrix->data);
}
int read_all_matrix(FILE *f, ordinary_matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < matrix->m; j++)
            if (fscanf(f, SC, &(matrix->ptrs[i][j])) != 1)
                return ERR;
    return OK;
}
void zero_ord_matrix(ordinary_matrix_t *m)
{
    memset(m->data, '\0', m->n * m->m * sizeof(elem_t));
}
void print_matrix(ordinary_matrix_t *matrix)
{
    if (matrix->n <= 30 && matrix->m <= 30)
        for (size_t i = 0; i < matrix->n; i++)
        {
            for (size_t j = 0; j < matrix->m; j++)
                printf(PR, matrix->ptrs[i][j]);
            printf("\n");
        }
    else
    {
        for (size_t i = 0; i < matrix->n; i++)
            for (size_t j = 0; j < matrix->m; j++)
                if (matrix->ptrs[i][j] != 0)
                {
                    printf("row of element: %zu\ncolumn of element: %zu\nelement: ", i + 1, j + 1);
                    printf(PR, matrix->ptrs[i][j]);
                    printf("\n\n");
                }
    }
}
int read_matrix_by_index(ordinary_matrix_t *matrix)
{
    long i, j;
    printf("if you want to finish, enter 0 when position in row is asked\n");
    printf("row of element: ");
    if (scanf("%ld", &i) != 1)
        return ERR;
    while (i != 0)
    {
        if (i < 0 || i > matrix->n)
        {
            printf("error while reading row\n");
            return ERR;
        }
        printf("column of element: ");
        if (scanf("%ld", &j) != 1 || j < 1 || j > matrix->m)
        {
            printf("error while reading column\n");
            return ERR;
        }
        printf("element: ");
        if (scanf(SC, &(matrix->ptrs[i - 1][j - 1])) != 1)
        {
            printf("error while reading element\n");
            return ERR;
        }
        printf("row of element: ");
        if (scanf("%ld", &i) != 1)
        {
            printf("error while reading row\n");
            return ERR;
        }
    }
    return OK;
}
void fill_matrix_auto_with_percentage(ordinary_matrix_t *matrix, double percentage)
{
    size_t pos_i, pos_j;
    long count = (long) (percentage * matrix->m * matrix->n / 100.0);
    while (count > 0)
    {
        pos_i = rand() % matrix->n;
        pos_j = rand() % matrix->m;
        if (matrix->ptrs[pos_i][pos_j] == 0)
        {
            matrix->ptrs[pos_i][pos_j] = rand() % MAX + 1;
            count--;
        }
    }
}
int read_vector(FILE *f, vector_t *v)
{
    for (size_t i = 0; i < v->size; i++)
        if (fscanf(f, SC, (elem_t *)element_at(v, i)) != 1)
            return ERR;
    return OK;
}
void print_vector(vector_t *v)
{
    if (v->size <= 30)
    {
        for (size_t i = 0; i < v->size; i++)
            printf(PR, *(elem_t *)element_at(v, i));
        printf("\n");
    }
    else
    {
        for (size_t i = 0; i < v->size; i++)
            if (*(elem_t *)element_at(v, i) != 0)
            {
                printf("column of element: %zu\nelement: ", i + 1);
                printf(PR, *(elem_t *)element_at(v, i));
                printf("\n\n");
            }
    }
}
void fill_zero_vec(vector_t *v)
{
    for (size_t i = 0; i < v->size; i++)
        *(size_t *)element_at(v, i) = 0;
}
void copy_matrix(ordinary_matrix_t *src, ordinary_matrix_t *dst)
{
    dst->n = src->n;
    dst->m = src->m;
    memmove(dst, src, src->n * src->m * sizeof(elem_t));
}
void multiply_vec_and_ord_matrix(vector_t *vec, ordinary_matrix_t *matrix, vector_t *res)
{
    assert(matrix->n == vec->size);
    assert(res->size == 0);
    fill_zero_vec(res);
    for (size_t i = 0; i < matrix->m; i++)
    {
        elem_t elem = 0;
        for (size_t j = 0; j < matrix->n; j++)
            elem += matrix->ptrs[j][i] * *(elem_t *)element_at(vec, j);
        push_back_to_vector(res, &elem);
    }
}
