#include "vector.h"
int alloc_vector(vector_t *v, size_t size_of_element, size_t size)
{
    if ((v->front = calloc(size, size_of_element)) == NULL)
        return MEMORY_ERROR;
    v->size_of_element = size_of_element;
    v->alloc_size = size;
    v->size = 0;
    return OK;
}
void free_vector(vector_t *v)
{
    free(v->front);
}
void zero_vector(vector_t *v)
{
    memset(v->front, '\0', v->alloc_size * v->size_of_element);
    v->size = 0;
}
void fill_vector_auto_with_percentage(vector_t *vec, double percentage)
{
    size_t pos;
    long count = (long) (percentage * vec->size / 100.0);
    while (count > 0)
    {
        pos = rand() % vec->size;
        if (*(elem_t *)element_at(vec, pos) == 0)
        {
            *(elem_t *)element_at(vec, pos) = rand() % MAX + 1;
            count--;
        }
    }
}
void * element_at(vector_t *v, size_t pos)
{
    return (((char *)v->front + pos * v->size_of_element));
}
int push_back_to_vector(vector_t *v, void *elem)
{
    if (v->size == v->alloc_size)
    {
        vector_t v_cp;
        if (alloc_vector(&v_cp, v->size_of_element, v->alloc_size) == MEMORY_ERROR)
            return MEMORY_ERROR;
        v_cp.size = v->size;
        memmove(((char *)v_cp.front), ((char *)v->front), v->size_of_element * v->size);
        free_vector(v);
        if (alloc_vector(v, v->size_of_element, v->size * 2) == MEMORY_ERROR)
            return MEMORY_ERROR;
        v->size = v_cp.size;
        memmove(((char *)v->front), ((char *)v_cp.front), v->size_of_element * v_cp.size);
        free_vector(&v_cp);
    }
    memmove(((char *)v->front) + v->size * v->size_of_element, elem, v->size_of_element);
    v->size++;
    return OK;
}
void read_vector_by_index(vector_t *v)
{
    long i;
    printf("if you want to finish, enter 0 when position in column is asked\n");
    printf("column of element: ");
    if (scanf("%ld", &i) != 1)
    {
        printf("error while reading column\n");
        return;
    }
    while (i != 0)
    {
        if (i <= 0 || i > v->size)
        {
            printf("error while reading column\n");
            return;
        }
        printf("element: ");
        if (scanf("%ld", (elem_t *)element_at(v, i - 1)) != 1)
        {
            printf("error while reading element\n");
            return;
        }
        printf("column of element: ");
        if (scanf("%ld", &i) != 1)
        {
            printf("error while reading column\n");
            return;
        }
    }
    return;
}
