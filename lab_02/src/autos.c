#include "autos.h"
size_t read_autos(FILE *f, automobile_t *a, size_t max)
{
    size_t count = 0;
    automobile_t au;
    while (count <= max)
    {
        if (read_auto(f, &au) != OK)
            break;
        if (count == max)
            return ERR_LEN;
        *a = au;
        a++;
        count++;
    }
    if (count == 0)
        return ERR_LEN;
    return count;
}
void print_autos(FILE *f, automobile_t *a, size_t n)
{
    
    for (size_t i = 0; i < n; i++)
    {
        printf("number: %ld\n", i + 1);
        print_auto(f, &a[i]);
    }
    printf("number of autos: %ld\n", n);
}
int add_auto(FILE *f, automobile_t *a, long *n, long max)
{
    if (*n < max)
    {
        if (read_auto(f, a + *n) != OK)
            return ERR;
        (*n)++;
    }
    else
        return ERR_LEN;
    return OK;
}
int delete_auto(automobile_t *a, long *n, long pos)
{
    if (*n <= 0 || pos >= *n)
        return ERR;
    for (long i = pos; i < *n - 1; i++)
        a[i] = a[i + 1];
    (*n)--;
    return OK;
}
int delete_autos(automobile_t *a, long *n, int (* need_delete)(automobile_t *, field_value_t *, field_t, size_t i), field_value_t *u, field_t field)
{
    long i = 0;
    while (i < *n)
    {
        if (need_delete(a + i, u, field, i))
        {
            delete_auto(a, n, i);
            if (field == NUMBER)
                break;
        }
        else
            i++;
    }
    return OK;
}
int filter_and_print_autos(FILE *f, automobile_t *a, size_t n, string_t trademark, double price_min, double price_max)
{
    int b = 0;
    for(size_t i = 0; i < n; i++)
    {
        if (a->condition_type == old && strcmp(a->trademark, trademark) == 0 && a->price >= price_min && a->price <= price_max && a->condition.old.owners == 1 && a->condition.old.repairs == 0)
        {
            print_auto(f, a);
            b = 1;
        }
        a++;
    }
    return b;
}
void make_keys_table(const automobile_t *a, size_t n, keys_t *keys)
{
    for(size_t i = 0; i < n; i++)
    {
        keys[i].num = i;
        keys[i].value = a[i].price;
    }
}
void copy_autos(const automobile_t *a_src, size_t n, automobile_t *a_dst)
{
    for (size_t i = 0; i < n; i++)
    {
        *a_dst = *a_src;
        a_src++;
        a_dst++;
    }
}
void copy_keys(const keys_t *a_src, size_t n, keys_t *a_dst)
{
    for (size_t i = 0; i < n; i++)
    {
        *a_dst = *a_src;
        a_src++;
        a_dst++;
    }
}
int comp_keys(const void *a, const void *b)
{
    keys_t *f = (keys_t *)a;
    keys_t *s = (keys_t *)b;
    if (f->value - s->value > EPS)
        return 1;
    else if (s->value - f->value > EPS)
        return -1;
    else
        return 0;
}
int comp_autos(const void *a, const void *b)
{
    automobile_t *f = (automobile_t *)a;
    automobile_t *s = (automobile_t *)b;
    if (f->price - s->price > EPS)
        return 1;
    else if (s->price - f->price > EPS)
        return -1;
    else
        return 0;
}
void print_autos_with_keys(FILE *f, automobile_t *a, size_t n, keys_t *keys)
{
    for (size_t i = 0; i < n; i++)
        print_auto(f, &a[keys[i].num]);
    printf("number of autos: %ld\n", n);
}
void print_keys(FILE *f, keys_t *keys, size_t n)
{
    for (size_t i = 0; i < n; i++)
        fprintf(f, "position: %zu; value: %lf\n", keys[i].num + 1, keys[i].value);
}
