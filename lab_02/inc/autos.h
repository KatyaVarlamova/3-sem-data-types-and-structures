#ifndef autos_h
#define autos_h
#include "auto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OK 0
#define ERR 1
#define ERR_LEN -1
#define EPS 1e-10
typedef struct
{
    size_t num;
    double value;
} keys_t;
typedef union
{
    double price;
    size_t num;
    string_t str;
} field_value_t;
typedef enum
{
    NUMBER, TRADEMARK, COUNTRY, PRICE, COLOR, CONDITION_TYPE
} field_t;
size_t read_autos(FILE *f, automobile_t *a, size_t max);
void print_autos(FILE *f, automobile_t *a, size_t n);
int add_auto(FILE *f, automobile_t *a, long *n, long max);
int delete_autos(automobile_t *a, long *n, int (* need_delete)(automobile_t *, field_value_t *, field_t, size_t i), field_value_t *u, field_t field);
int filter_and_print_autos(FILE *f, automobile_t *a, size_t n, string_t trademark, double price_min, double price_max);
void make_keys_table(const automobile_t *a, size_t n, keys_t *keys);
void copy_autos(const automobile_t *a_src, size_t n, automobile_t *a_dst);
void copy_keys(const keys_t *a_src, size_t n, keys_t *a_dst);
int comp_keys(const void *a, const void *b);
int comp_autos(const void *a, const void *b);
void print_autos_with_keys(FILE *f, automobile_t *a, size_t n, keys_t *keys);
void print_keys(FILE *f, keys_t *keys, size_t n);
#endif
