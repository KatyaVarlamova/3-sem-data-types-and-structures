#include "hash_table.h"
#define HASH_FUNC_TYPES_SIZE 3
char xor_rands[256];
static hash_func_type_t hash_func_types_arr[HASH_FUNC_TYPES_SIZE] = {hash_func_sum, hash_func_mul, hash_func_xor};
static int is_prime(size_t n)
{
    if (n <= 1)
        return 0;
    size_t i;
    i = 2;
    while (i < n / 2 + 1)
    {
        if (n % i == 0)
            return 0;
        i++;
    }
    return 1;
}
static size_t find_next_prime(size_t n)
{
    size_t next = n;
    while (!is_prime(next++));
    next--;
    assert(is_prime(next));
    return next;
}
size_t hash_func_sum(elem_t *elem, hash_table_t *table)
{
    size_t key = get_key(elem);
    return key % table->size;
}
size_t hash_func_mul(elem_t *elem, hash_table_t *table)
{
    size_t key = get_key(elem);
    double num = (table->mul_const * key);
    double dr = num - (size_t)(num);
    size_t r = (size_t)(table->size * dr);
    return r;
}
size_t hash_func_xor(elem_t *elem, hash_table_t *table)
{
    size_t key = get_xor_key(elem, xor_rands);
    return key % table->size;
}
hash_table_t * create_hash_table(size_t max_count, hash_func_type_t hash_func)
{
    max_count = find_next_prime(max_count);
    hash_table_t *table = malloc(sizeof(hash_table_t));
    if (table)
    {
        table->data = calloc(max_count, sizeof(list_t));
        table->size = max_count;
        table->mul_const = (sqrt(5) - 1.0) / 2.0;
        table->hash_func_type = hash_func;
        table->av_conflicts = 0;
    }
    return table;
}
void free_hash_table(hash_table_t **table, void (* free_elem)(void *val))
{
    for (size_t i = 0; i < (*table)->size; i++)
        free_list(&((*table)->data[i].head), free_elem);
    free((*table)->data);
    free(*table);
    *table = NULL;
}
void set_av_conflicts(hash_table_t *table)
{
    size_t count = 0;
    double sum = 0;
    for (size_t i = 0; i < table->size; i++)
        if (table->data[i].head != NULL)
        {
            count++;
            sum += table->data[i].count;
        }
    table->av_conflicts = sum / count;
}
void read_hash_table(char *filename, hash_table_t *table, void *(*reader)(FILE *f), stat_values_t *stat)
{
    FILE *f = fopen(filename, "r");
    elem_t *el;
    while ((el = reader(f)) != NULL)
    {
        insert_hash_table_elem(table, el);
        stat->elems_count++;
    }
    set_av_conflicts(table);
    fclose(f);
}
void print_hash_table(FILE *f, hash_table_t *table, void print_elem(FILE *f, const void *elem))
{
    printf("average conflicts: %.2lf\n", table->av_conflicts);
    printf("current size: %ld\n", table->size);
    printf("current hash func: ");
    if (table->hash_func_type == hash_func_sum)
        printf("sum\n");
    if (table->hash_func_type == hash_func_mul)
        printf("mul\n");
    if (table->hash_func_type == hash_func_xor)
        printf("xor\n");
    for (size_t i = 0; i < table->size; i++)
        if (table->data[i].head != NULL)
        {
            printf("index: %zu\nvalues in list: ", i);
            print_list(f, table->data[i].head, print_elem);
            printf("\n");
        }
}
void delete_hash_table_elem(hash_table_t *table, elem_t *elem, void (* free_elem)(void *val), int (* comp)(const void *lhs, const void *rhs), stat_values_t *stat)
{
    elem_t *del_elem;
    size_t ind = table->hash_func_type(elem, table);
    if (table->data[ind].head != NULL)
    {
        del_elem = find_and_delete(&table->data[ind].head, elem, comp, &stat->comp_count);
        free_elem(del_elem);
    }
}
void insert_hash_table_elem(hash_table_t *table, elem_t *value)
{
    size_t ind = table->hash_func_type(value, table);
    node_t node;
    node.data = value;
    push_front(&(table->data[ind].head), &node);
    table->data[ind].count++;
}
hash_table_t * reduce_conflicts_by_restruction_ht(hash_table_t *table, double av_max)
{
#define max_iterations 100
    size_t table_size = table->size;
    double min = table->av_conflicts;
    hash_func_type_t hf_min = table->hash_func_type;
    size_t table_size_min = table->size;
    size_t max_iter = 0;
    while (table->av_conflicts > av_max && max_iter < 100)
    {
        for (size_t i  = 0; table->av_conflicts > av_max && i < HASH_FUNC_TYPES_SIZE; i++)
        {
            table = restruct(table, hash_func_types_arr[i], table_size);
            set_av_conflicts(table);
            if (table->av_conflicts < min)
            {
                hf_min = table->hash_func_type;
                table_size_min = table->size;
                min = table->av_conflicts;
            }
        }
        table_size = find_next_prime(table_size + 1);
        max_iter++;
    }
    if (max_iter == max_iterations)
    {
        table = restruct(table, hf_min, table_size_min);
        table->hash_func_type = hf_min;
        set_av_conflicts(table);
        assert(table->av_conflicts == min);
    }
    return table;
}
hash_table_t * restruct(hash_table_t *table, hash_func_type_t hash_func, size_t new_size)
{
    elem_t *elem;
    hash_table_t *new_table = create_hash_table(new_size, hash_func);
    for (size_t i = 0; i < table->size; i++)
        while (table->data[i].head != NULL)
        {
            elem = pop_front(&table->data[i].head);
            insert_hash_table_elem(new_table, elem);
        }
    set_av_conflicts(new_table);
    free_hash_table(&table, NULL);
    return new_table;
}
