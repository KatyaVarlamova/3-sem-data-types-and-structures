#include "queue_on_list.h"
void init_queue_on_list(queue_on_list_t *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    alloc_vector(&queue->adrs, sizeof(void *), INIT_SIZE);
}
void delete_queue_on_list(queue_on_list_t *queue, void (* free_elem)(elem_t *))
{
    free_vector(&queue->adrs);
    node_t *next;
    for (; queue->head != NULL; queue->head = next)
    {
        next = queue->head->next;
        if (free_elem)
            free_elem(queue->head->value);
        free(queue->head);
    }
}
int push_to_queue_on_list(void *q, void *value)
{
    queue_on_list_t *queue = q;
    node_t *new_node = calloc(1, sizeof(node_t));
    if (new_node == NULL)
        return MEMORY_ERROR;
    if (queue->tail)
        queue->tail->next = new_node;
    else
        queue->head = new_node;
    new_node->next = NULL;
    new_node->value = value;
    queue->tail = new_node;
    queue->count++;
    return OK;
}
void * get_front_in_queue_on_list(void *q)
{
    queue_on_list_t *queue = q;
    if (queue->head == NULL)
        return NULL;
    return queue->head->value;
}
int insert_to_queue_on_list(void *q, void *value)
{
#define MAX_POS 4
    queue_on_list_t *queue = q;
    node_t *new_node = calloc(1, sizeof(node_t));
    if (new_node == NULL)
        return MEMORY_ERROR;
    new_node->value = value;
    new_node->next = NULL;
    size_t i = 0;
    if (queue->head == NULL)
    {
        queue->head = new_node;
        queue->tail = new_node;
        queue->count++;
        return OK;
    }
    node_t *cur, *tmp;
    for (cur = queue->head; i < MAX_POS - 2 && cur->next != NULL; cur = cur->next, i++);
    if (cur == queue->tail)
    {
        queue->count++;
        cur->next = new_node;
        queue->tail = new_node;
        return OK;
    }
    tmp = cur->next;
    cur->next = new_node;
    new_node->next = tmp;
    queue->count++;
    return OK;
}
size_t get_length_in_list(void *q)
{
    queue_on_list_t *queue = q;
    return queue->count;
}
void * pop_from_queue_on_list(void *q)
{
    queue_on_list_t *queue = q;
    if (queue->head == NULL && queue->tail == NULL)
        return NULL;
    void *value = queue->head->value;
    void *adr = queue->head;
    push_back_to_vector(&queue->adrs, &adr);
    node_t *head = queue->head;
    if (queue->head == queue->tail)
        queue->tail = NULL;
    queue->head = queue->head->next;
    free(head);
    queue->count--;
    return value;
}
int read_queue_on_list(FILE *f, queue_on_list_t *queue, elem_t * (* reader)(FILE *))
{
    elem_t *value = NULL;
    while ((value = reader(f)) != NULL)
        if (push_to_queue_on_list(queue, value) != OK)
            return LENGTH_ERROR;
    return OK;
}
void print_queue_on_list(FILE *f, void *q, void (* printer)(FILE *, const elem_t *))
{
    queue_on_list_t *queue = q;
    for (node_t *cur = queue->head; cur != NULL; cur = cur->next)
    {
        printf("element: ");
        printer(f, cur->value);
        printf("located: %p\n", cur);
    }
    printf("free: ");
    print_vector_st(&queue->adrs);
}
