#include "vector.h"
vector_t * alloc_vector(size_t size)
{
    vector_t *v = malloc(sizeof(vector_t));
    if (!v)
        return NULL;
    if ((v->front = calloc(size, sizeof(elem_t *))) == NULL)
    {
        free(v);
        return NULL;
    }
    v->alloc_size = size;
    v->size = 0;
    return v;
}
void free_vector(vector_t *v, void (* free_elem)(void *val))
{
    for (size_t i = 0; i < v->size; i++)
        free_elem(v->front[i]);
    free(v->front);
    v->alloc_size = 0;
    v->size = 0;
    free(v);
}
void read_vector(FILE *f, vector_t *vector, void *(*reader)(FILE *f, size_t max), size_t max)
{
    elem_t *elem;
    while ((elem = reader(f, max)) != NULL)
        push_back_to_vector(vector, elem);
}
static void delete_elem_at(vector_t *vector, size_t pos)
{
    free_elem(vector->front[pos]);
    for (long i = pos; i < vector->size; i++)
        vector->front[i] = vector->front[i + 1];
    vector->size--;
}
void delete_elem_in_vector(vector_t *vector, elem_t *value, int (* comp)(const void *lhs, const void *rhs), void (* free_elem)(void *val))
{
    for (size_t i = 0; i < vector->size; i++)
        if (comp(vector->front[i], value) == 0)
        {
            delete_elem_at(vector, i);
            break;
        }
}
void print_vector(FILE *f, vector_t *vector, void print_elem(FILE *f, const void *elem))
{
    for (size_t i = 0; i < vector->size; i++)
        print_elem(f, vector->front[i]);
}
int push_back_to_vector(vector_t *v, elem_t *elem)
{
    if (v->size == v->alloc_size)
    {
        if (v->front == NULL || v->alloc_size == 0)
            return MEMORY_ERROR;
        void *temp = realloc(v->front, sizeof(elem_t *) * v->alloc_size * 2);
        if (temp == NULL)
            return MEMORY_ERROR;
        v->front = temp;
        v->alloc_size = v->size * 2;
    }
    v->front[v->size] = elem;
    v->size++;
    return OK;
}
int resize(vector_t *v, size_t new_size)
{
    if (v->front == NULL || new_size == 0)
        return MEMORY_ERROR;
    void *temp = realloc(v->front, sizeof(elem_t) * new_size);
    if (temp == NULL)
        return MEMORY_ERROR;
    v->front = temp;
    v->alloc_size = new_size;
    return OK;
}
void to_dot(char *name, vector_t *vec)
{
    FILE *f = fopen(name ,"w");
    fprintf(f, "digraph test_tree {\n");
    for (size_t i = 0; i < vec->size; i++)
        fprintf(f, "%d -> %d [label = %d]\n", vec->front[i]->src, vec->front[i]->dst, vec->front[i]->cost);
    fprintf(f, "}\n");
    fclose(f);
}
