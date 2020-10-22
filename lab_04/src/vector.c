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
    v->size_of_element = 0;
    v->alloc_size = 0;
    v->size = 0;
}
void zero_vector(vector_t *v)
{
    memset(v->front, '\0', v->alloc_size * v->size_of_element);
    v->size = 0;
}
int push_back_to_vector(vector_t *v, void *elem)
{
    if (v->size == v->alloc_size)
    {
        vector_t v_cp;
        if (alloc_vector(&v_cp, v->size_of_element, v->alloc_size) == MEMORY_ERROR)
            return MEMORY_ERROR;
        v_cp.size = v->size;
        v_cp.size_of_element = v->size_of_element;
        memmove(((char *)v_cp.front), ((char *)v->front), v->size_of_element * v->size);
        free_vector(v);
        if (alloc_vector(v, v_cp.size_of_element, v_cp.size * 2) == MEMORY_ERROR)
            return MEMORY_ERROR;
        v->size = v_cp.size;
        memmove(((char *)v->front), ((char *)v_cp.front), v->size_of_element * v_cp.size);
        free_vector(&v_cp);
    }
    memmove(((char *)v->front) + v->size * v->size_of_element, elem, v->size_of_element);
    v->size++;
    return OK;
}
void pop_from_vector(vector_t *v)
{
    v->size--;
}
void print_vector(vector_t *v)
{
    for (size_t i = 0 ; i < v->size; i++)
        printf(PR, *((char *)v->front + i * v->size_of_element));
    printf("\n\n");
}
void print_vector_st(vector_t *v)
{
    for (size_t i = 0 ; i < v->size; i++)
        printf("%p ", *((void **)v->front + i));
    printf("\n\n");
}

