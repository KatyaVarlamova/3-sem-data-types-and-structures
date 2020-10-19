#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ordinary_matrix.h"
#include "sparse_matrix.h"
#include "sparse_vector.h"
#include "vector.h"
#define OK                   0
#define ERR                  1
#define ERR_READ             2
#define INIT_PERCENTAGE      40

#define EXIT                 0
#define AUTO_MATRIX          1
#define MANUALLY_ALL_MATRIX  2
#define MANUALLY_SOME_MATRIX 3
#define AUTO_VECTOR          4
#define MANUALLY_ALL_VECTOR  5
#define MANUALLY_SOME_VECTOR 6
#define CHANGE_PERCENTAGE_M  7
#define CHANGE_PERCENTAGE_V  8
#define COUNT_ORDINARY       9
#define COUNT_SPARSE         10
#define STATISTICS           11
#define OUT_ORD_VEC          12
#define OUT_ORD_MATR         13
#define OUT_SP_VEC           14
#define OUT_SP_MATR          15
void free_resources(sparse_matrix_t *sm, sparse_vector_t *sv, sparse_vector_t *res, ordinary_matrix_t *om, vector_t *ov, vector_t *ores)
{
    free_sparse_matrix(sm);
    free_sparse_vector(sv);
    free_sparse_vector(res);
    free_ord_matrix(om);
    free_vector(ov);
    free_vector(ores);
}
int init(sparse_matrix_t *sm, sparse_vector_t *sv, sparse_vector_t *res, ordinary_matrix_t *om, vector_t *ov, vector_t *ores, size_t n, size_t m)
{
    if (alloc_sparse_matrix(sm) != OK)
    {
        free_resources(sm, sv, res, om, ov, ores);
        return ERR;
    }
    if (alloc_sparse_vector(sv) != OK)
    {
        free_resources(sm, sv, res, om, ov, ores);
        return ERR;
    }
    if (alloc_sparse_vector(res) != OK)
    {
        free_resources(sm, sv, res, om, ov, ores);
        return ERR;
    }
    if (alloc_ord_matrix(om, n, m) != OK)
    {
        free_resources(sm, sv, res, om, ov, ores);
        return ERR;
    }
    if (alloc_vector(ov, sizeof(elem_t), n) != OK)
    {
        free_resources(sm, sv, res, om, ov, ores);
        return ERR;
    }
    if (alloc_vector(ores, sizeof(elem_t), 10) != OK)
    {
        free_resources(sm, sv, res, om, ov, ores);
        return ERR;
    }
    res->n = 0;
    ores->size = 0;
    sv->n = n;
    ov->size = n;
    sm->n = n;
    sm->m = m;
    om->n = n;
    om->m = m;
    return OK;
}
void zero_everything(sparse_matrix_t *sm, sparse_vector_t *sv, sparse_vector_t *res, ordinary_matrix_t *om, vector_t *ov, vector_t *ores, long n, long m)
{
    zero_sparse_matrix(sm);
    zero_ord_matrix(om);
    zero_vector(ov);
    zero_sparse_vector(sv);
    zero_sparse_vector(res);
    zero_vector(ores);
    res->n = 0;
    ores->size = 0;
    sv->n = n;
    ov->size = n;
    sm->n = n;
    sm->m = m;
    om->n = n;
    om->m = m;
}
void zero_matrix(sparse_matrix_t *sm, sparse_vector_t *res, ordinary_matrix_t *om, vector_t *ores, long n, long m)
{
    zero_sparse_matrix(sm);
    zero_ord_matrix(om);
    zero_sparse_vector(res);
    zero_vector(ores);
    res->n = 0;
    ores->size = 0;
    sm->n = n;
    sm->m = m;
    om->n = n;
    om->m = m;
}
void zero_vectors(sparse_vector_t *sv, sparse_vector_t *res, vector_t *ov, vector_t *ores, long n, long m)
{
    zero_vector(ov);
    zero_sparse_vector(sv);
    zero_sparse_vector(res);
    zero_vector(ores);
    res->n = 0;
    ores->size = 0;
    sv->n = n;
    ov->size = n;
}
void statistics(long n, long m)
{
    sparse_matrix_t sm;
    sparse_vector_t sv, res;
    ordinary_matrix_t om;
    vector_t ov, ores;
    if (init(&sm, &sv, &res, &om, &ov, &ores, n, m) != OK)
        return;
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    printf("x--------------x------------x------------x-------------x-------------x\n");
    printf("|    percent   |  ordinary  |   sparse   |   size_or   |   size_sp   |\n");
    printf("x--------------x------------x------------x-------------x-------------x\n");
char *p ="\nx--------------x------------x------------x-------------x-------------x\n";
    size_t s = 0, s_o;
    s_o = ov.size * ov.size_of_element + om.n * om.n * sizeof(elem_t);
    for (int i = 1; i <= 100; i += 5)
    {
        time = 0;
        printf("|%14d|", i);
        zero_everything(&sm, &sv, &res, &om, &ov, &ores, n, m);
        fill_matrix_auto_with_percentage(&om, i);
        copy_sparse_matrix(&om, &sm);
        fill_vector_auto_with_percentage(&ov, i);
        copy_sparse_vector(&ov, &sv);
        for (size_t j = 0; j < 1; j++)
        {
            zero_vector(&ores);
            ores.size = 0;
            gettimeofday(&tv_start, NULL);
            multiply_vec_and_ord_matrix(&ov, &om, &ores);
            gettimeofday(&tv_stop, NULL);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        printf("%12.2lf|", time / 1.0);
        time = 0;
        s =  sv.columns.size * sv.columns.size_of_element +
        sv.data.size * sv.data.size_of_element +
        sm.columns.size * sm.columns.size_of_element +
        sm.rows.size * sm.rows.size_of_element +
        sm.data.size * sm.data.size_of_element +
        sm.n * sizeof(size_t);
        for (size_t j = 0; j < 1; j++)
        {
            zero_sparse_vector(&res);
            res.n = 0;
            gettimeofday(&tv_start, NULL);
            multiply_vec_and_matrix(&sv, &sm, &res);
            gettimeofday(&tv_stop, NULL);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        printf("%12.2lf|", time / 1.0);
        printf("%13zu|", s_o);
        printf("%13zu|", s);
        printf("%s", p);
    }
}
void print_menu()
{
    printf(
        "EXIT                      0\n"
        "AUTO FILL MATRIX          1\n"
        "MANUALLY FILL ALL MATRIX  2\n"
        "MANUALLY FILL SOME MATRIX 3\n"
        "AUTO FILL VECTOR          4\n"
        "MANUALLY FILL ALL VECTOR  5\n"
        "MANUALLY FILL SOME VECTOR 6\n"
        "CHANGE PERCENTAGE MATRIX  7\n"
        "CHANGE PERCENTAGE VECTOR  8\n"
        "COUNT ORDINARY            9\n"
        "COUNT SPARSE              10\n"
        "STATISTICS                11\n"
        "OUT ORDINARY VECTOR       12\n"
        "OUT ORDINARY MATRIX       13\n"
        "OUT SPARSE VECTOR         14\n"
        "OUT SPARSE MATRIX         15\n");
}
int main(void)
{
    int choise;
    sparse_matrix_t sm;
    sparse_vector_t sv, res;
    ordinary_matrix_t om;
    vector_t ov, ores;
    long n, m, n1, m1;
    printf("Program multiplies a vector by a matrix.\n");
    printf("NOTE: matrix and vector are filled automatically by default. If you want to specify them, please, look menu.\n");
    printf("input number of rows: ");
    if (scanf("%ld", &n) != 1 || n <= 0)
    {
        printf("error while reading\n");
        return ERR;
    }
    printf("input number of columns: ");
    if (scanf("%ld", &m) != 1 || m <= 0)
    {
        printf("error while reading\n");
        return ERR_READ;
    }
    if (init(&sm, &sv, &res, &om, &ov, &ores, n, m) != OK)
    {
        printf("errors with memory\n");
        return ERR;
    }
    print_menu();
    printf("action: ");
    if (scanf("%d", &choise) != 1)
    {
        printf("errors while reading\n");
        return ERR_READ;
    }
    double percentage_matrix = INIT_PERCENTAGE;
    double percentage_vector = INIT_PERCENTAGE;
    zero_everything(&sm, &sv, &res, &om, &ov, &ores, n, m);
    zero_everything(&sm, &sv, &res, &om, &ov, &ores, n, m);
    fill_matrix_auto_with_percentage(&om, percentage_matrix);
    fill_vector_auto_with_percentage(&ov, percentage_vector);
    copy_sparse_vector(&ov, &sv);
    copy_sparse_matrix(&om, &sm);
    while (choise != EXIT)
    {
        switch (choise)
        {
            case AUTO_MATRIX:
                zero_matrix(&sm, &res, &om, &ores, n, m);
                fill_matrix_auto_with_percentage(&om, percentage_matrix);
                copy_sparse_matrix(&om, &sm);
                break;
            case AUTO_VECTOR:
                zero_vectors(&sv, &res, &ov, &ores, n, m);
                fill_vector_auto_with_percentage(&ov, percentage_vector);
                copy_sparse_vector(&ov, &sv);
                break;
            case MANUALLY_ALL_MATRIX:
                zero_matrix(&sm, &res, &om, &ores, n, m);
                if (read_all_matrix(stdin, &om) != OK)
                    printf("error while reading\n");
                copy_sparse_matrix(&om, &sm);
                break;
            case MANUALLY_ALL_VECTOR:
                zero_vectors(&sv, &res, &ov, &ores, n, m);
                if (read_vector(stdin, &ov) != OK)
                    printf("error while reading\n");
                copy_sparse_vector(&ov, &sv);
                break;
            case MANUALLY_SOME_MATRIX:
                zero_matrix(&sm, &res, &om, &ores, n, m);
                read_matrix_by_index(&om);
                copy_sparse_matrix(&om, &sm);
                break;
            case MANUALLY_SOME_VECTOR:
                zero_vectors(&sv, &res, &ov, &ores, n, m);
                read_vector_by_index(&ov);
                copy_sparse_vector(&ov, &sv);
                break;
            case CHANGE_PERCENTAGE_M:
                printf("input persentage of filling matrix: ");
                if (scanf("%lf", &percentage_matrix) != 1)
                    printf("error while reading\n");
                break;
            case CHANGE_PERCENTAGE_V:
                printf("input persentage of filling vector: ");
                if (scanf("%lf", &percentage_vector) != 1)
                    printf("error while reading\n");
                break;
            case COUNT_ORDINARY:
                multiply_vec_and_ord_matrix(&ov, &om, &ores);
                print_vector(&ores);
                break;
            case COUNT_SPARSE:
                zero_sparse_vector(&res);
                res.n = 0;
                multiply_vec_and_matrix(&sv, &sm, &res);
                print_sparse_vector(&res);
                break;
            case STATISTICS:
                printf("input number of rows: ");
                if (scanf("%ld", &n1) != 1 || n1 <= 0)
                    printf("error while reading\n");
                printf("input number of columns: ");
                if (scanf("%ld", &m1) != 1 || m1 <= 0)
                    printf("error while reading\n");
                statistics(n1, m1);
                break;
            case OUT_ORD_MATR:
                print_matrix(&om);
                break;
            case OUT_ORD_VEC:
                print_vector(&ov);
                break;
            case OUT_SP_MATR:
                print_sparse_matrix(&sm);
                break;
            case OUT_SP_VEC:
                print_sparse_vector(&sv);
                break;
            default:
                printf("there is no such action\n");
        }
        print_menu();
        printf("action: ");
        if (scanf("%d", &choise) != 1)
        {
            printf("errors while reading\n");
            return ERR_READ;
        }
    }
    return 0;
}
