#ifndef stack_on_array_h
#define stack_on_array_h

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "element.h"
#define MEMORY_ERROR 1
#define LENGTH_ERROR 2
#define OK 0
#define INIT_SIZE 32
typedef struct {
    vector_t data;
    elem_t *head;
    size_t count;
    size_t max_count;
    size_t size_of_element;
} stack_on_array_t;
int init_stack_on_array(stack_on_array_t *stack, size_t max_count);
void delete_stack_on_array(stack_on_array_t *stack);
int push_to_stack_on_array(void *st, void *value);
int pop_from_stack_on_array(void *st, void *value);
void print_stack_on_array(void *st);
#endif 
