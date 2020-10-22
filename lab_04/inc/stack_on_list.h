#ifndef stack_on_list_h
#define stack_on_list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "vector.h"
#define MEMORY_ERROR 1
#define LENGTH_ERROR 2
#define OK 0
#define INIT_SIZE 4
typedef struct node
{
    struct node *next;
    elem_t value;
} node_t;
typedef struct
{
    node_t *head;
    size_t count;
    size_t max_count;
    size_t size_of_element;
    vector_t adrs;
} stack_on_list_t;
void init_stack_on_list(stack_on_list_t *stack, size_t max_count);
void delete_stack_on_list(stack_on_list_t *stack);
int push_to_stack_on_list(void *st, void *value);
void print_stack_on_list(void *st);
int pop_from_stack_on_list(void *st, void *value);
#endif
