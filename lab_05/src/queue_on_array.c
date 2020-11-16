#include "queue_on_array.h"
int init_queue_on_array(queue_on_array_t *queue, size_t max_count)
{
    queue->data = calloc(max_count, sizeof(elem_t *));
    if (queue->data == NULL)
        return MEMORY_ERROR;
    queue->max_count = max_count;
    queue->count = 0;
    queue->head = 0;
    queue->tail = 0;
    return OK;
}
int read_queue_on_array(FILE *f, queue_on_array_t *queue, elem_t * (* reader)(FILE *))
{
    elem_t *value = NULL;
    while ((value = reader(f)) != NULL)
        if (push_to_queue_on_array(queue, value) != OK)
            return LENGTH_ERROR;
    return OK;
}
void * get_front_in_queue_on_array(void *q)
{
    queue_on_array_t *queue = q;
    if (queue->count == 0)
        return NULL;
    return queue->data[queue->head];
}
int insert_to_queue_on_array(void *q, void *value)
{
#define MAX_POS 4
    queue_on_array_t *queue = q;
//    printf("head_ins: %zu\n", queue->head);
//    printf("tail_ins: %zu\n", queue->tail);
//    printf("count_ins: %zu\n", queue->count);
    if (queue->count == queue->max_count)
        return LENGTH_ERROR;
    if (queue->count == 0)
    {
        queue->data[queue->head] = value;
        queue->count++;
        return OK;
    }
    size_t i = 0;
    for (i = 0; i < queue->count && i < MAX_POS - 1; i++);
    if (queue->head == 0)
        queue->data[queue->max_count - 1] = queue->data[0];
    else
         queue->data[(queue->head - 1) % (queue->max_count)] = queue->data[(queue->head) % (queue->max_count)];
    for (size_t j = 0; j < i - 1; j++)
    {
        queue->data[(queue->head + j) % (queue->max_count)] = queue->data[(queue->head + j + 1) % (queue->max_count)];
    }
    queue->data[(queue->head + i - 1) % (queue->max_count)] = (elem_t *)value;
    if (queue->head == 0)
        queue->head = queue->max_count - 1;
    else
        queue->head = (queue->head - 1) % (queue->max_count);
    queue->count++;
    return OK;
}
size_t get_length_of_array(void *q)
{
    queue_on_array_t *queue = q;
    return queue->count;
}
size_t get_head(queue_on_array_t *queue)
{
    return queue->head;
}
void delete_queue_on_array(queue_on_array_t *queue, void (* free_elem)(elem_t *))
{
    if (free_elem)
        for (size_t i = 0; i < queue->count; i++)
            free_elem(queue->data[(queue->head + i) % (queue->max_count)]);
    free(queue->data);
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    queue->max_count = 0;
}
int push_to_queue_on_array(void *q, void *value_pointer)
{
    queue_on_array_t *queue = q;
    if (queue->count == queue->max_count)
        return LENGTH_ERROR;
    if (queue->count == 0)
    {
        queue->data[queue->tail] = (elem_t *)value_pointer;
        queue->count++;
        return OK;
    }
    queue->tail = (queue->tail + 1) % queue->max_count;
    queue->data[queue->tail] = (elem_t *)value_pointer;
    queue->count++;
    return OK;
}
void * pop_from_queue_on_array(void *q)
{
    queue_on_array_t *queue = q;
    if (queue->count == 0)
        return NULL;
    elem_t *value = queue->data[queue->head];
    if (queue->head == queue->tail)
        queue->tail = (queue->head + 1) % queue->max_count;
    queue->head = (queue->head + 1) % queue->max_count;
    queue->count--;
    return value;
}
void print_queue_on_array(FILE *f, void *q, void (* printer)(FILE *, const elem_t *))
{
    queue_on_array_t *queue = q;
    for (size_t i = 0; i < queue->count; i++)
        printer(f, queue->data[(queue->head + i) % (queue->max_count)]);
//    printf("count_pr: %zu\nhead_pr: %zu\ntail_pr: %zu\n", queue->count, queue->head, queue->tail);
}
