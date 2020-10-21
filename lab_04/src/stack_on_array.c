#include "stack_on_array.h"
int init_stack_on_array(stack_on_array_t *stack, size_t max_count)
{
    stack->size_of_element = sizeof(elem_t);
    if (alloc_vector(&stack->data, stack->size_of_element, INIT_SIZE))
        return MEMORY_ERROR;
    stack->max_count = max_count;
    stack->count = 0;
    stack->head = NULL;
    return OK;
}
void delete_stack_on_array(stack_on_array_t *stack)
{
    free_vector(&stack->data);
    stack->head = NULL;
    stack->count = 0;
    stack->max_count = 0;
}
int push_to_stack_on_array(void *st, void *value)
{
    stack_on_array_t *stack = st;
    if (stack->count == stack->max_count)
        return LENGTH_ERROR;
    if (push_back_to_vector(&stack->data, value) != OK)
        return MEMORY_ERROR;
    stack->head = stack->data.front + stack->count;
    stack->count++;
    return OK;
}
int pop_from_stack_on_array(void *st, void *value)
{
    stack_on_array_t *stack = st;
    if (stack->count == 0)
        return LENGTH_ERROR;
    memmove(value, stack->head, stack->size_of_element);
    if (stack->count == 1)
        stack->head = NULL;
    else
        stack->head--;
    pop_from_vector(&stack->data);
    stack->count--;
    return OK;
}
void print_stack_on_array(void *st)
{
    stack_on_array_t *stack = st;
    elem_t *cur = stack->head;
    for (size_t i = 0; i < stack->count; cur--, i++)
        printf(PR, *cur);
    printf("\n");
}
