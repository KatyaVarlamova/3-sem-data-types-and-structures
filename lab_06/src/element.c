#include "element.h"
void print_elem(FILE *f, const void *el)
{
    const elem_t *elem = el;
    fprintf(f, "%s ", elem->value);
}
int read_str(FILE *f, char* s, size_t max)
{
    size_t i = 0;
    int c = fgetc(f);
    while (c == '\n')
        c = fgetc(f);
    while (c != ' ' && c != '\n' && c != EOF)
    {
        if (i < max - 1)
            s[i++] = c;
        else
            return ERR;
        c = fgetc(f);
    }
    s[i] = '\0';
    if (i == 0)
        return ERR;
    return OK;
}
void *reader_elem(FILE *f)
{
    elem_t *elem = malloc(sizeof(elem_t));
    if (read_str(f, elem->value, MAXLEN) != OK)
    {
        free(elem);
        return NULL;
    }
    return elem;
}
int compare_elems(const void *l, const void *r)
{
    const elem_t *lhs = l, *rhs = r;
    return strcmp(lhs->value, rhs->value);
}
void free_elem(void *val)
{
    free(val);
}
void generate_xor_rands(xor_rands_t xor_rands)
{
    for (size_t i = 0; i < MAXLEN; i++)
        xor_rands[i] = (rand() % 1280) / 10;
}
static size_t get_xor_sum(char *s, xor_rands_t xor_rands)
{
    size_t sum = 0;
    for (size_t i = 0; s[i] != '\0'; i++)
        sum += s[i] ^ xor_rands[i];
    return sum;
}
size_t get_xor_key(elem_t *elem, xor_rands_t xor_rands)
{
    return get_xor_sum(elem->value, xor_rands);
}
static size_t get_sum(char *s)
{
    size_t sum = 0;
    for (; *s != '\0'; s++)
        sum += *s;
    return sum;
}
size_t get_key(elem_t *elem)
{
    return get_sum(elem->value);
}
//size_t get_key(elem_t *elem)
//{
//    return (size_t)elem->value;
//}
//void print_elem(FILE *f, const void *el)
//{
//    const elem_t *elem = el;
//    fprintf(f, PR, elem->value);
//}
//void *reader_elem(FILE *f)
//{
//    elem_t *elem = malloc(sizeof(elem_t));
//    if (fscanf(f, SC, &elem->value) != 1)
//    {
//        free(elem);
//        return NULL;
//    }
//    return elem;
//}
//int compare_elems(const void *l, const void *r)
//{
//    const elem_t *lhs = l, *rhs = r;
//    return lhs->value - rhs->value;
//}
//void free_elem(void *val)
//{
//    free(val);
//}
