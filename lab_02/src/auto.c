#include "auto.h"
long read_str(FILE *f, string_t s, size_t max)
{
    size_t i = 0;
    int c = fgetc(f);
    while (c == '\n')
        c = fgetc(f);
    while (c != '\n' && c != EOF)
    {
        if (i < max - 1)
            s[i++] = c;
        else
            return ERR_LEN;
        c = fgetc(f);
    }
    s[i] = '\0';
    if (i == 0)
        return EMPTY;
    return i;
}
int read_auto(FILE *f, automobile_t *a)
{
    if (read_str(f, a->trademark, MAXLEN) < 0)
        return ERR;
    if (read_str(f, a->country, MAXLEN) < 0)
        return ERR;
    if (fscanf(f, "%lf", &(a->price)) != 1)
        return ERR;
    if (read_str(f, a->color, MAXLEN) < 0)
        return ERR;
    string_t condition;
    if (read_str(f, condition, MAXLEN) < 0)
        return ERR;
    if (strcmp(condition, "old") == 0)
        a->condition_type = old;
    else if (strcmp(condition, "new") == 0)
        a->condition_type = new;
    else
        return ERR;
    if (a->condition_type == old)
    {
        if (fscanf(f, "%ld", &((a->condition).old.year)) != 1)
            return ERR;
        if (fscanf(f, "%ld", &((a->condition).old.mileage)) != 1)
            return ERR;
        if (fscanf(f, "%ld", &((a->condition).old.repairs)) != 1)
            return ERR;
        if (fscanf(f, "%ld", &((a->condition).old.owners)) != 1)
            return ERR;
    }
    else
        if (fscanf(f, "%ld", &((a->condition).guarantee)) != 1)
            return ERR;
    
    return OK;
}
void print_auto(FILE *f, automobile_t *a)
{
    fprintf(f, "trademark: %s\n", a->trademark);
    fprintf(f, "country: %s\n", a->country);
    fprintf(f, "price: %lf\n", a->price);
    fprintf(f, "color: %s\n", a->color);
    if (a->condition_type == old)
    {
        fprintf(f, "year: %ld\n", (a->condition).old.year);
        fprintf(f, "mileage: %ld\n", (a->condition).old.mileage);
        fprintf(f, "repairs: %ld\n", (a->condition).old.repairs);
        fprintf(f, "owners: %ld\n", (a->condition).old.owners);
    }
    else
        fprintf(f, "guarantee: %ld\n", (a->condition).guarantee);
    fprintf(f, "\n");
}

