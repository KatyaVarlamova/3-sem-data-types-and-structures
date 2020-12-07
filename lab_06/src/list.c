#include "list.h"
node_t *create_node(void *data)
{
    node_t *new_node = calloc(1, sizeof(node_t));
    if (new_node == NULL)
        return NULL;
    new_node->data = data;
    return new_node;
}
void * find_and_delete(node_t **head, const void *value, int (*comparator)(const void *, const void *), int *comp_count)
{
    if (head == NULL || *head == NULL)
        return NULL;
    node_t *cur = *head, *prev = *head;
    for (*comp_count = 1; cur && comparator(value, cur->data); prev = cur, cur = cur->next, (*comp_count)++);
    void *data;
    if (cur == *head)
    {
        data = (*head)->data;
        *head = (*head)->next;
        free(cur);
        return data;
    }
    if (cur == NULL)
        return NULL;
    data = cur->data;
    prev->next = cur->next;
    free(cur);
    return data;
}
node_t *find(node_t *head, const void *data, int (*comparator)(const void *, const void *), int *comp_count)
{
    if (!head)
        return NULL;
    for (*comp_count = 0; head && comparator(data, head->data); head = head->next, (*comp_count)++);
    return head;
}
int push_front(node_t **head, node_t *elem)
{
    node_t *new_node = calloc(1, sizeof(node_t));
    if (new_node == NULL)
        return MEMORY_ERROR;
    new_node->data = elem->data;
    new_node->next = *head;
    *head = new_node;
    return OK;
}
int push_back(node_t **head, node_t *elem)
{
    node_t *new_node = calloc(1, sizeof(node_t));
    new_node->data = elem->data;
    if (new_node == NULL)
        return MEMORY_ERROR;
    node_t *cur = *head;
    if ((*head))
    {
        for (; cur->next; cur = cur->next);
        cur->next = new_node;
    }
    else
        *head = new_node;
    return OK;
}
void *pop_front(node_t **head)
{
    if (!head || !*head)
        return NULL;
    node_t *head_prev = *head;
    *head = (*head)->next;
    void *data = head_prev->data;
    free(head_prev);
    return data;
}
void *pop_back(node_t **head)
{
    if (!head || !*head)
        return NULL;
    node_t *cur = *head;
    void *data;
    if (((*head)->next) == NULL)
    {
        *head = NULL;
        data = cur->data;
        free(cur);
        return data;
    }
    for (; cur->next->next; cur = cur->next);
    data = cur->next->data;
    free(cur->next);
    cur->next = NULL;
    return data;
}
void free_list(node_t **head, void (*free_object)(void *))
{
    if (!*head)
        return;
    node_t *tmp;
    while ((*head)->next)
    {
        if (free_object != NULL)
            free_object((*head)->data);
        (*head)->data = NULL;
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
    if (free_object != NULL)
        free_object((*head)->data);
    (*head)->data = NULL;
    free(*head);
    *head = NULL;
}
void print_list(FILE *f, node_t *head, void (*print_element)(FILE *, const void *))
{
    for (; head; head = head->next)
        print_element(f, head->data);
    fprintf(f, "\n");
}
node_t *reverse(node_t *head)
{
    if (head == NULL)
        return NULL;
    node_t *cur = head->next, *prev = head, *tmp;
    head->next = NULL;
    while (cur)
    {
        tmp = cur->next;
        cur->next = prev;
        prev = cur;
        cur = tmp;
    }
    return prev;
}
void insert(node_t **head, node_t *elem, node_t *before)
{
    if (head == NULL || elem == NULL)
        return;
    if (*head == NULL || before == *head)
    {
        elem->next = *head;
        *head = elem;
        return;
    }
    node_t *cur = *head;
    for (; cur && cur->next != before; cur = cur->next);
    if (!cur)
        return;
    cur->next = elem;
    elem->next = before;
}
void *delete_node(node_t **head, node_t **elem)
{
    if (head == NULL || elem == NULL || *head == NULL || *elem == NULL)
        return NULL;
    void *data;
    node_t *cur;
    if (*elem == *head)
    {
        data = (*head)->data;
        *head = (*head)->next;
        free(*elem);
        *elem = NULL;
        return data;
    }
    for (cur = *head; cur && cur->next != *elem; cur = cur->next);
    if (cur == NULL)
        return NULL;
    data = (*elem)->data;
    cur->next = (*elem)->next;
    free(*elem);
    *elem = NULL;
    return data;
}
void front_back_split(node_t *head, node_t **back)
{
    if (head == NULL)
        return;
    node_t *cur = head;
    size_t n = 0;
    for (; cur; cur = cur->next, n++);
    n -= n / 2;
    cur = head;
    for (size_t i = 0; i < n - 1; cur = cur->next, i++);
    *back = cur->next;
    cur->next = NULL;
}
node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*comparator)(const void *, const void *))
{
    if (!head_a || !head_b || comparator == NULL)
        return NULL;
    node_t *sorted = NULL, *tmp;
    while (*head_a != NULL && *head_b != NULL)
    {
        if (comparator((*head_a)->data, (*head_b)->data) <= 0)
        {
            push_front(&sorted, *head_a);
            tmp = *head_a;
            *head_a = (*head_a)->next;
            free(tmp);
        }
        else
        {
            push_front(&sorted, *head_b);
            tmp = *head_b;
            *head_b = (*head_b)->next;
            free(tmp);
        }
    }
    while (*head_a != NULL)
    {
        push_front(&sorted, *head_a);
        tmp = *head_a;
        *head_a = (*head_a)->next;
        free(tmp);
    }
    while (*head_b != NULL)
    {
        push_front(&sorted, *head_b);
        tmp = *head_b;
        *head_b = (*head_b)->next;
        free(tmp);
    }
    sorted = reverse(sorted);
    return sorted;
}
node_t *copy(node_t *head)
{
    node_t *copied_head = NULL;
    for (; head; head = head->next)
        if (push_front(&copied_head, head) != OK)
        {
            free_list(&copied_head, NULL);
            return NULL;
        }
    copied_head = reverse(copied_head);
    return copied_head;
}
node_t *sort(node_t *head, int (*comparator)(const void *, const void *))
{
    if (head == NULL || comparator == NULL)
        return NULL;
    node_t *back = NULL;
    if (head->next == NULL)
        return sorted_merge(&head, &back, comparator);
    front_back_split(head, &back);
    head = sort(head, comparator);
    back = sort(back, comparator);
    return sorted_merge(&head, &back, comparator);
}
node_t *read_list(FILE *f, void *(*reader)(FILE *))
{
    node_t node, *head = NULL;
    void *data;
    while ((data = reader(f)) != NULL)
    {
        node.data = data;
        node.next = NULL;
        push_front(&head, &node);
    }
    head = reverse(head);
    return head;
}
//    node_t *val = malloc(sizeof(node_t));
//    if (val == NULL)
//        return;
//    memmove(val, elem, sizeof(node_t));
//    if (*head == NULL || before == *head)
//    {
//        elem->next = *head;
//        *head = elem;
//    }
//    else
//    {
//        node_t tmp;
//        memmove(&tmp, elem, sizeof(node_t));
//        memmove(elem, before, sizeof(node_t));
//        memmove(before, &tmp, sizeof(node_t));
//        before->next = elem;
//    }
