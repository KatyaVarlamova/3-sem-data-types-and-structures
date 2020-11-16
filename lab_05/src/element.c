#include "element.h"

void print_elem(FILE *f, const elem_t *elem)
{
    fprintf(f, PR" type: %d", elem->value, elem->type);
}
elem_t *reader_elem(FILE *f)
{
    elem_t *elem = malloc(sizeof(elem_t));
    if (fscanf(f, SC, &elem->value) != 1)
    {
        free(elem);
        return NULL;
    }
    elem->procesing_time = 0;
    elem->begin_time = 0;
    elem->type = FIRST;
    return elem;
}
void free_elem(elem_t *val)
{
    free(val);
}
