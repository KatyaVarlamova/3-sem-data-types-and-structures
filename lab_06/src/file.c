#include "file.h"
char * make_file_copy(char *filename, void *(*reader)(FILE *f), void print_elem(FILE *f, const void *elem))
{
    FILE *f = fopen(filename, "r");
    char *file_name_copy = malloc(strlen(filename) + 6);
    strcpy(file_name_copy, filename);
    strcat(file_name_copy, "_copy");
    FILE *copy = fopen(file_name_copy, "w");
    elem_t *elem;
    while ((elem = reader(f)) != NULL)
    {
        print_elem(copy, elem);
        free_elem(elem);
    }
    fclose(f);
    fclose(copy);
    return file_name_copy;
}
void print_file(FILE *out, const char *filename, void *(*reader)(FILE *f), void print_elem(FILE *f, const void *elem))
{
    FILE *f = fopen(filename, "r");
    elem_t *elem;
    while ((elem = reader(f)) != NULL)
    {
        print_elem(out, elem);
        free_elem(elem);
    }
    fprintf(out, "\n");
    fclose(f);
}
void free_filename(char *file_name)
{
    free(file_name);
}
void delete_from_file(const char *filename, elem_t *elem, void *(*reader)(FILE *f), void print_elem(FILE *f, const void *elem), int (* comp)(const void *lhs, const void *rhs), stat_values_t *stat)
{
    char *tmp_name = malloc(strlen(filename) + 6);
    strcpy(tmp_name, filename);
    strcat(tmp_name, "_tmp");
    FILE *tmp = fopen(tmp_name, "w");
    FILE *f = fopen(filename, "r");
    elem_t *el;
    while ((el = reader(f)) != NULL)
    {
        stat->comp_count++;
        stat->elems_count++;
        if (comp(el, elem) != 0)
            print_elem(tmp, el);
        free_elem(el);
    }
    fclose(f);
    fclose(tmp);
    f = fopen(filename, "w");
    print_file(f, tmp_name, reader, print_elem);
    free_filename(tmp_name);
    fclose(f);
}
