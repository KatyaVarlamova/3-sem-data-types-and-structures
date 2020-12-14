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
    int src, dst, cost;
} elem_t;

void *reader_elem(FILE *f, size_t max);

void free_elem(void *val);

#endif
