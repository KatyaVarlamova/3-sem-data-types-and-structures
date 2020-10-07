#ifndef auto_h
#define auto_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OK 0
#define ERR 1
#define ERR_LEN -1
#define EMPTY -2
#define MAXLEN 30
typedef char string_t[MAXLEN];
typedef struct
{
    long year;
    long mileage;
    long repairs;
    long owners;
} old_auto_t;
typedef union
{
    old_auto_t old;
    long guarantee;
} condition_t;
typedef enum
{
    old, new
} condition_type_t;
typedef struct
{
    string_t trademark;
    string_t country;
    double price;
    string_t color;
    condition_type_t condition_type;
    condition_t condition;
} automobile_t;
long read_str(FILE *f, string_t s, size_t max);
int read_auto(FILE *f, automobile_t *a);
void print_auto(FILE *f, automobile_t *a);
#endif
