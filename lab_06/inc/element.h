#ifndef element_h
#define element_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 30
#define SC "%s"
#define PR "%s "
#define OK 0
#define ERR 2

typedef struct
{
    char value[MAXLEN];
} elem_t;

typedef struct
{
    int comp_count;
    size_t elems_count;
} stat_values_t;

void print_elem(FILE *f, const void *el);

void *reader_elem(FILE *f);

size_t get_key(elem_t *elem);

int compare_elems(const void *lhs, const void *rhs);

void free_elem(void *val);

#endif
