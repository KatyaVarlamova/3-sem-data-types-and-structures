#include "stack_on_list.h"
void init_stack_on_list(stack_on_list_t *stack, size_t max_count)
{
    stack->max_count = max_count;
    stack->head = NULL;
    stack->count = 0;
    stack->size_of_element = sizeof(elem_t);
    alloc_vector(&stack->adrs, sizeof(void *), INIT_SIZE);
}
void delete_stack_on_list(stack_on_list_t *stack)
{
    free_vector(&stack->adrs);
    node_t *next;
    for (;stack->head != NULL; stack->head = next)
    {
        next = stack->head->next;
        free(stack->head);
    }
}
int push_to_stack_on_list(void *st, void *value)
{
    stack_on_list_t *stack = st;
    if (stack->count == stack->max_count)
        return LENGTH_ERROR;
    node_t *new_node = calloc(1, sizeof(node_t));
    if (new_node == NULL)
        return MEMORY_ERROR;
    new_node->next = stack->head;
    memmove(&new_node->value, value, stack->size_of_element);
    stack->head = new_node;
    stack->count++;
    return OK;
}
int pop_from_stack_on_list(void *st, void *value)
{
    stack_on_list_t *stack = st;
    if (stack->count == 0)
        return LENGTH_ERROR;
    memmove(value, &stack->head->value, stack->size_of_element);
    void *adr = stack->head;
    push_back_to_vector(&stack->adrs, &adr);
    node_t *head = stack->head;
    stack->head = stack->head->next;
    free(head);
    stack->count--;
    return OK;
}
void print_stack_on_list(void *st)
{
    stack_on_list_t *stack = st;
    for (node_t *cur = stack->head; cur != NULL; cur = cur->next)
    {
        printf("element: "PR"located: %p\n", cur->value, cur);
    }
    printf("free: ");
    print_vector_st(&stack->adrs);
}
