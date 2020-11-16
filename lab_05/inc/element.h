#ifndef element_h
#define element_h

#include <stdio.h>
#include <stdlib.h>
#define SC "%d"
#define PR "%d "
typedef enum
{
    FIRST, SECOND
} type_t;
typedef struct
{
    double begin_time;
    double procesing_time;
    int value;
    type_t type;
} elem_t;

void print_elem(FILE *f, const elem_t *elem);

elem_t *reader_elem(FILE *f);

void free_elem(elem_t *val);

#endif
