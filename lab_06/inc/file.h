#ifndef file_h
#define file_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"

#define OK           0
#define MEMORY_ERROR 1

char * make_file_copy(char *filename, void *(*reader)(FILE *f), void print_elem(FILE *f, const void *elem));

void print_file(FILE *out, const char *filename, void *(*reader)(FILE *f), void print_elem(FILE *f, const void *elem));

void delete_from_file(const char *filename, elem_t *elem, void *(*reader)(FILE *f), void print_elem(FILE *f, const void *elem), int (* comp)(const void *lhs, const void *rhs), stat_values_t *stat);

void free_filename(char *file_name);

#endif
