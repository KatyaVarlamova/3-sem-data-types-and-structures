#ifndef sort_h
#define sort_h

#include <stdio.h>
void min_max_sort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *));
void heap_sort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *));
void quicksort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *));
#endif
