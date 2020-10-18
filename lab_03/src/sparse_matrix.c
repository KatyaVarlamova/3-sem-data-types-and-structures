#include "sparse_matrix.h"
int alloc_sparse_matrix(sparse_matrix_t *matrix)
{
    if (alloc_vector(&matrix->data, sizeof(elem_t), INIT_SIZE) == MEMORY_ERROR)
        return MEMORY_ERROR;
    if (alloc_vector(&matrix->columns, sizeof(size_t), INIT_SIZE) == MEMORY_ERROR)
    {
        free_vector(&matrix->data);
        return MEMORY_ERROR;
    }
    if (alloc_vector(&matrix->rows,  sizeof(size_t), INIT_SIZE) == MEMORY_ERROR)
    {
        free_vector(&matrix->data);
        free_vector(&matrix->columns);
        return MEMORY_ERROR;
    }
    matrix->n = 0;
    matrix->m = 0;
    return OK;
}
void free_sparse_matrix(sparse_matrix_t *matrix)
{
    free_vector(&matrix->data);
    free_vector(&matrix->columns);
    free_vector(&matrix->rows);
}
void zero_sparse_matrix(sparse_matrix_t *m)
{
    zero_vector(&m->columns);
    zero_vector(&m->rows);
    zero_vector(&m->data);
    m->n = 0;
    m->m = 0;
}
int read_all_sparse_matrix(FILE *f, sparse_matrix_t *matrix)
{
    size_t pos = 0;
    elem_t a;
    for (size_t i = 0; i < matrix->n; i++)
    {
        push_back_to_vector(&matrix->rows, &pos);
        for (size_t j = 0; j < matrix->m; j++)
        {
            if (fscanf(f, SC, &a) != 1)
                return ERR;
            if (a != 0)
            {
                push_back_to_vector(&matrix->data, &a);
                push_back_to_vector(&matrix->columns, &j);
                pos++;
            }
        }
    }
    push_back_to_vector(&matrix->rows, &pos);
    return OK;
}
int copy_sparse_matrix(ordinary_matrix_t *om, sparse_matrix_t *matrix)
{
    size_t pos = 0;
    for (size_t i = 0; i < matrix->n; i++)
    {
        push_back_to_vector(&matrix->rows, &pos);
        for (size_t j = 0; j < matrix->m; j++)
        {
            if (om->ptrs[i][j] != 0)
            {
                push_back_to_vector(&matrix->data, &om->ptrs[i][j]);
                push_back_to_vector(&matrix->columns, &j);
                pos++;
            }
        }
    }
    push_back_to_vector(&matrix->rows, &pos);
    return OK;
}
void print_sparse_matrix(sparse_matrix_t *matrix)
{
    printf("data: \n");
    for (size_t i = 0; i < matrix->data.size; i++)
        printf(PR, *((elem_t *)element_at(&matrix->data, i)));
    printf("\ncolumns: \n");
    for (size_t i = 0; i < matrix->columns.size; i++)
        printf("%zu ", *((size_t *)element_at(&matrix->columns, i)));
    printf("\npointers: \n");
    for (size_t i = 0; i < matrix->rows.size; i++)
        printf("%ld " , *((size_t *)element_at(&matrix->rows, i)));
    printf("\n");
}
size_t size_t_at(vector_t *v, size_t pos)
{

    return *(size_t *)element_at(v, pos);
}
elem_t elem_t_at(vector_t *v, size_t pos)
{
    return *(elem_t *)element_at(v, pos);
}
void fill_zero_vector(vector_t *v)
{
    for (size_t i = 0; i < v->size; i++)
        *(size_t *)element_at(v, i) = 0;
}
//void print_size_t_vector(vector_t *v)
//{
//    for (size_t i = 0; i < v->size; i++)
//        printf("%zu ", size_t_at(v, i));
//    printf("\n");
//}
void multiply_vec_and_matrix(sparse_vector_t *vec, sparse_matrix_t *matrix, sparse_vector_t *res)
{
    assert(matrix->rows.size - 1 == matrix->n);
    vector_t storage;
    alloc_vector(&storage, sizeof(size_t), vec->n);
    storage.size = vec->n;
    fill_zero_vector(&storage);
    elem_t *ptr = calloc(matrix->m, sizeof(elem_t));
    for (size_t j = 0; j < vec->columns.size; j++)
        *(size_t *)element_at(&storage, size_t_at(&vec->columns, j)) = j + 1;
    for (size_t j = 0; j < matrix->rows.size - 1; j++) // row
    {
        for (size_t k = size_t_at(&matrix->rows, j); k < size_t_at(&matrix->rows, j + 1); k++)
            if (size_t_at(&storage, j) != 0)
                ptr[size_t_at(&matrix->columns, k)] += elem_t_at(&matrix->data, k) * elem_t_at(&vec->data, size_t_at(&storage, j) - 1);
    }
    for (size_t i = 0; i < matrix->m; i++)
        if (ptr[i] != 0)
        {
            push_back_to_vector(&res->data, &ptr[i]);
            push_back_to_vector(&res->columns, &i);
        }
    free(ptr);
    free_vector(&storage);
}
