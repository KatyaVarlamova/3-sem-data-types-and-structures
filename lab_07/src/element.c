#include "element.h"
void *reader_elem(FILE *f, size_t max)
{
    elem_t *elem = malloc(sizeof(elem_t));
    if (fscanf(f, "%d", &elem->src) != 1 || elem->src < 0 || elem->src > max)
    {
        free_elem(elem);
        return NULL;
    }
    if (fscanf(f, "%d", &elem->dst) != 1 || elem->dst < 0 || elem->src > max)
    {
        free_elem(elem);
        return NULL;
    }
    if (fscanf(f, "%d", &elem->cost) != 1)
    {
        free_elem(elem);
        return NULL;
    }
    return elem;
}
void free_elem(void *val)
{
    free(val);
}
