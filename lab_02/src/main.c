#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "autos.h"
#include "sort.h"
#define OK 0
#define DEBUG 0
#define ERR 1
#define ERR_LEN -1
#define ERR_FILE 2
#define EMPTY -2
#define MEMORY_ERROR 3
#define MAXLEN 30
#define MAXCOUNT 100

void print_time(void (* func) (void *, size_t, size_t, int (* comparator) (const void *, const void *)), const void * m, size_t n, size_t s, int (* comparator) (const void *, const void *))
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0, time_make = 0;
    automobile_t ac[MAXCOUNT];
    keys_t k[MAXCOUNT];
    keys_t k_copy[MAXCOUNT];
    if (comparator == comp_keys)
    {
        gettimeofday(&tv_start, NULL);
        make_keys_table((automobile_t *) m, n, k);
        gettimeofday(&tv_stop, NULL);
        time_make = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        copy_keys(k, n, k_copy);
    }
    else
        copy_autos(m, n, ac);
    for (size_t i = 0; i < 1000; i++)
    {
        if (comparator == comp_keys)
        {
            gettimeofday(&tv_start, NULL);
            func(k, n, s, comparator);
            gettimeofday(&tv_stop, NULL);
            copy_keys(m, n, k);
            time += time_make + (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        else
        {
            gettimeofday(&tv_start, NULL);
            func(ac, n, s, comparator);
            gettimeofday(&tv_stop, NULL);
            copy_autos(m, n, ac);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
    }
    printf("  %10.2lf|", time / 1000.0);
}
void print_statistics(automobile_t *a, size_t n)
{
    printf("x--------------x------------x------------x------------x\n");
    printf("|              |  heapsort  |   minmax   |   qsort    |\n");
    printf("x--------------x------------x------------x------------x\n");
    printf("|  with keys   |");
    print_time(heap_sort, a, n, sizeof(keys_t), comp_keys);
    print_time(min_max_sort, a, n, sizeof(keys_t), comp_keys);
    print_time(quicksort, a, n, sizeof(keys_t), comp_keys);
    printf("\nx--------------x------------x------------x------------x\n");
    printf("| without keys |");
    print_time(heap_sort, a, n, sizeof(automobile_t), comp_autos);
    print_time(min_max_sort, a, n, sizeof(automobile_t), comp_autos);
    print_time(quicksort, a, n, sizeof(automobile_t), comp_autos);
    printf("\nx--------------x------------x------------x------------x\n");
    printf("size of table: %ld, size of keys: %ld\n", n * sizeof(automobile_t), n * sizeof(keys_t));
}
int need_delete(automobile_t * a, field_value_t *u, field_t f, size_t i)
{
    if (f == NUMBER)
    {
        printf("%ld %ld\n", u->num, i);
        return u->num - 1 == i;
    }
    else if (f == TRADEMARK)
        return !strcmp(a->trademark, u->str);
    else if (f == COUNTRY)
        return !strcmp(a->country, u->str);
    else if (f == COLOR)
        return !strcmp(a->color, u->str);
    else if (f == CONDITION_TYPE)
    {
        if (strcmp(u->str, "old") == 0 && a->condition_type == old)
            return 1;
        else if (strcmp(u->str, "new") == 0 && a->condition_type == new)
            return 1;
        else return 0;
    }
    else if (f == PRICE)
    {
        if (a->price - u->price > EPS || u->price - a->price > EPS)
            return 0;
        else
            return 1;
    }
    else
        return 0;
}
void print_str(FILE *f, char *s)
{
    if (!DEBUG)
        fprintf(f, "%s", s);
}
int filter_dialog(FILE *f, FILE *f_out, string_t trademark, double *p_min, double *p_max)
{
    print_str(f_out, "trademark: ");
    if (read_str(f, trademark, MAXLEN) == ERR_LEN)
        return ERR;
    double temp;
    print_str(f_out, "range of price:\n");
    print_str(f_out, "min: ");
    string_t s;
    if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%lf", p_min) != 1)
        return ERR;
    print_str(f_out, "max: ");
    if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%lf", p_max) != 1)
        return ERR;
    if (*p_min - *p_max > EPS)
    {
        temp = *p_min;
        *p_min = *p_max;
        *p_max = temp;
    }
    return OK;
}
int add_dialog(FILE *f, FILE *f_out, automobile_t *a)
{
    string_t s;
    print_str(f_out, "trademark: ");
    if (read_str(f, a->trademark, MAXLEN) < 0)
    {
        print_str(f_out, "error while reading\n");
        return ERR;
    }
    print_str(f_out, "country: ");
    if (read_str(f, a->country, MAXLEN) < 0)
    {
        print_str(f_out, "error while reading\n");
        return ERR;
    }
    print_str(f_out, "price: ");
    if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%lf", &(a->price)) != 1)
    {
        print_str(f_out, "error while reading\n");
        return ERR;
    }
    print_str(f_out, "color: ");
    if (read_str(f, a->color, MAXLEN) < 0)
    {
        print_str(f_out, "error while reading\n");
        return ERR;
    }
    string_t condition;
    print_str(f_out, "condition type (old / new): ");
    if (read_str(f, condition, MAXLEN) < 0)
        print_str(f_out, "error while reading\n");
    if (strcmp(condition, "old") == 0)
        a->condition_type = old;
    else if (strcmp(condition, "new") == 0)
        a->condition_type = new;
    else
    {
        print_str(f_out, "error while reading\n");
        return ERR;
    }
    if (a->condition_type == old)
    {
        print_str(f_out, "year: ");
        if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%ld", &((a->condition).old.year)) != 1)
        {
            print_str(f_out, "error while reading\n");
            return ERR;
        }
        print_str(f_out, "mileage: ");
        if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%ld", &((a->condition).old.mileage)) != 1)
        {
            print_str(f_out, "error while reading\n ");
            return ERR;
        }
        print_str(f_out, "repairs: ");
        if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%ld", &((a->condition).old.repairs)) != 1)
        {
            print_str(f_out, "error while reading\n ");
            return ERR;
        }
        print_str(f_out, "owners: ");
        while (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%ld", &((a->condition).old.owners)) != 1)
        {
            print_str(f_out, "error while reading\n");
            return ERR;
        }
    }
    else
    {
        print_str(f_out, "guarantee: ");
        if (read_str(f, s, MAXLEN) < 0 || sscanf(s, "%ld", &((a->condition).guarantee)) != 1)
        {
            print_str(f_out, "error while reading\n");
            return ERR;
        }
    }
    return OK;
}
int delete_dialog(FILE *f, FILE *f_out, automobile_t *a, long *n)
{
    string_t s;
    print_str(f_out, "Choose one of the following field names which will be key for deletion (just write name of field): \n* number\n* trademark\n* country\n* price\n* color\n* condition type\n");
    if (read_str(f, s, MAXLEN) == ERR_LEN)
        return ERR;
    field_t field; field_value_t u;
    if (strcmp(s, "number") == 0)
    {
        print_str(f_out, "number: ");
        field = NUMBER;
    }
    else if (strcmp(s, "trademark") == 0)
    {
        print_str(f_out, "trademark: ");
        field = TRADEMARK;
    } else if (strcmp(s, "country") == 0)
    {
        print_str(f_out, "country: ");
        field = COUNTRY;
    } else if (strcmp(s, "color") == 0)
    {
        print_str(f_out, "color: ");
        field = COLOR;
    } else if (strcmp(s, "condition type") == 0)
    {
        print_str(f_out, "condition type (old / new): ");
        field = CONDITION_TYPE;
    } else if (strcmp(s, "price") == 0)
    {
        print_str(f_out, "price: ");
        field = PRICE;
    } else
        return ERR;
    if (field == PRICE)
        fscanf(f, "%lf", &u.price);
    else if (field == NUMBER)
        fscanf(f, "%ld", &u.num);
    else if (read_str(f, u.str, MAXLEN) == ERR_LEN)
            return ERR;
    if (delete_autos(a, n, need_delete, &u, field))
        return ERR;
    if (*n > 0)
        print_autos(f_out, a, *n);
    else
        return ERR_LEN;
    return OK;
}
int main(int argc, char **argv)
{
    string_t s;
    automobile_t automobiles[MAXCOUNT], automobiles_copy[MAXCOUNT];
    print_str(stdout, "Prepare file named file.txt with characteristics of auto in following order (each characteristic in a new line; it is possible to put empty line between autos):\n* trademark (1)\n* country (2)\n* price (3)\n* color (4)\n* old / new (5)\n* if last is new\n  > guarantee in years (6)\n* if last is old:\n  > year (6)\n  > mileage in km (7)\n  > quantity of repairs (8)\n  > quantity of ownwers (9)\n");
    if (argc < 2)
        return ERR;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        print_str(stdout, "no such file\n");
        return ERR_FILE;
    }
    long n = read_autos(f, automobiles, MAXCOUNT);
    if (n < 0)
    {
        print_str(stdout, "error while reading file\n");
        return ERR;
    }
    fclose(f);
    f = stdin;
    keys_t keys[MAXCOUNT];
    int choise;
        print_str(stdout, "File was successfully read. Now you can choose action with information in file (just write number of action)\n");
        print_str(stdout, "* 1 - find old autos with 1 last owner and with no repairs\n");
        print_str(stdout, "* 2 - add auto\n");
        print_str(stdout, "* 3 - delete auto\n");
        print_str(stdout, "* 41 - sort autos with key table (simple sort)\n");
        print_str(stdout, "* 42 - sort autos with key table (heap sort)\n");
        print_str(stdout, "* 43 - sort autos with key table (quick sort)\n");
        print_str(stdout, "* 51 - sort autos (simple sort)\n");
        print_str(stdout, "* 52 - sort autos (heap sort)\n");
        print_str(stdout, "* 53 - sort autos (quick sort)\n");
        print_str(stdout, "* 6 - print statistics with comparison of sort methods\n");
        print_str(stdout, "* 7 - print autos\n");
        print_str(stdout, "* 8 - print keys\n");
        print_str(stdout, "* 9 - print autos with keys\n");
        print_str(stdout, "* 10 - reread file\n\n");
        print_str(stdout, "number of action: ");
    if (scanf("%d", &choise) != 1)
    {
        print_str(stdout, "nothing was chosen");
        return ERR;
    }
    make_keys_table(automobiles, n, keys);
    while (choise != 0)
    {
        switch (choise) {
            case 1:
            {
                string_t trademark;
                double p_min, p_max;
                if (filter_dialog(f, stdout, trademark, &p_min, &p_max))
                {
                    print_str(stdout, "error while reading filter values\n");
                    break;
                }
                if (!filter_and_print_autos(stdout, automobiles, n, trademark, p_min, p_max))
                    print_str(stdout, "No results\n");
                break;
            }
            case 2:
            {
                if (n == MAXCOUNT)
                {
                    print_str(stdout, "impossible to add\n");
                    break;
                }
                if (add_dialog(f, stdout, automobiles + n) == OK)
                {
                    print_str(stdout, "successfully added!\n");
                    n++;
                }
                break;
            }
            case 3:
            {
                int err;
                if ((err = delete_dialog(f, stdout, automobiles, &n)))
                {
                    if (err == ERR_LEN)
                        print_str(stdout, "all elements were deleted\n");
                    else
                        print_str(stdout, "error while reading type of field or value of field\n");
                    break;
                }
                make_keys_table(automobiles, n, keys);
                break;
            }
            case 41:
            {
                min_max_sort(keys, n, sizeof(keys_t), comp_keys);
                break;
            }
            case 42:
            {
                heap_sort(keys, n, sizeof(keys_t), comp_keys);
                break;
            }
            case 43:
            {
                quicksort(keys, n, sizeof(keys_t), comp_keys);
                break;
            }
            case 51:
            {
                min_max_sort(automobiles, n, sizeof(automobile_t), comp_autos);
                make_keys_table(automobiles, n, keys);
                break;
            }
            case 52:
            {
                heap_sort(automobiles, n, sizeof(automobile_t), comp_autos);
                make_keys_table(automobiles, n, keys);
                break;
            }
            case 53:
            {
                quicksort(automobiles, n, sizeof(automobile_t), comp_autos);
                make_keys_table(automobiles, n, keys);
                break;
            }
            case 6:
            {
                copy_autos(automobiles, n, automobiles_copy);
                print_statistics(automobiles_copy, n);
                break;
            }
            case 7:
            {
                print_autos(stdout, automobiles, n);
                break;
            }
            case 8:
            {
                print_keys(stdout, keys, n);
                break;
            }
            case 9:
            {
                print_autos_with_keys(stdout, automobiles, n, keys);
                break;
            }
            case 10:
            {
                FILE *file = fopen(argv[1], "r");
                read_autos(file, automobiles, MAXCOUNT);
                fclose(file);
                make_keys_table(automobiles, n, keys);
                break;
            }
            default:
            {
                print_str(stdout, "Incorrect number of action\n");
                break;
            }
        }
        print_str(stdout, "number of action: ");
        while (read_str(f, s, MAXLEN) < 0 || sscanf(s,"%d", &choise) != 1)
            print_str(stdout, "Incorrect number of action\nnumber of action: ");
    }
    return OK;
}

