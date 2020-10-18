#include "sparse_vector.h"
int alloc_sparse_vector(sparse_vector_t *vec)
{
    if (alloc_vector(&vec->data, sizeof(elem_t), INIT_SIZE) == MEMORY_ERROR)
        return MEMORY_ERROR;
    if (alloc_vector(&vec->columns, sizeof(size_t), INIT_SIZE) == MEMORY_ERROR)
    {
        free_vector(&vec->data);
        return MEMORY_ERROR;
    }
    vec->n = 0;
    return OK;
}
void free_sparse_vector(sparse_vector_t *vec)
{
    free_vector(&vec->data);
    free_vector(&vec->columns);
}
void zero_sparse_vector(sparse_vector_t *v)
{
    zero_vector(&v->columns);
    zero_vector(&v->data);
    v->n = 0;
}
int read_all_sparse_vector(FILE *f, sparse_vector_t *vec)
{
    elem_t a;
    for (size_t j = 0; j < vec->n; j++)
    {
        if (fscanf(f, SC, &a) != 1)
            return ERR;
        if (a != 0)
        {
            push_back_to_vector(&vec->data, &a);
            push_back_to_vector(&vec->columns, &j);
        }
    }
    return OK;
}
int copy_sparse_vector(vector_t *ov, sparse_vector_t *vec)
{
    elem_t a;
    for (size_t j = 0; j < ov->size; j++)
    {
        a = *(elem_t *)element_at(ov, j);
        if (a != 0)
        {
            push_back_to_vector(&vec->data, &a);
            push_back_to_vector(&vec->columns, &j);
        }
    }
    return OK;
}
void print_sparse_vector(sparse_vector_t *vec)
{
    printf("data: \n");
    for (size_t i = 0; i < vec->data.size; i++)
        printf(PR, *((elem_t *)element_at(&vec->data, i)));
    printf("\ncolumns: \n");
    for (size_t i = 0; i < vec->columns.size; i++)
        printf("%zu ", *((size_t *)element_at(&vec->columns, i)) + 1);
    printf("\n");
}
