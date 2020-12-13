
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>

#include "vector.h"

#define INIT_SIZE 16
// /usr/local/Cellar/graphviz/2.44.1/bin:/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
const int INF = INT_MAX;

int min(int a, int b)
{
    return (a < b) ? a : b;
}
int * solve(vector_t *e, size_t n, size_t v, int *cycle_vertex) {
    size_t m = e->size;
    int *d = calloc(n + 1, sizeof(int));
    for (size_t i = 0; i < n + 1; i++)
        d[i] = INF;
    d[v] = 0;
    *cycle_vertex = -1;
    for (int i = 0; i < n; ++i)
    {
        *cycle_vertex = -1;
        for (int j = 0; j < m; ++j)
            if (d[e->front[j]->src] < INF)
            {
                d[e->front[j]->dst] = min (d[e->front[j]->dst], d[e->front[j]->src] + e->front[j]->cost);
                *cycle_vertex = e->front[j]->dst;
            }
    }
    return d;
}
int main(int argc, char **argv)
{
    printf("program finds all vertices of the graph that can be reached from a given vertex on a path no longer than A. \nList of vertices must be given in file. \n");
    if (argc < 2)
        return ERR;
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        printf("error while opening file\n");
        return ERR;
    }
    size_t n, v;
    printf("input source vertex: ");
    if (scanf("%zu", &v) != 1)
    {
        printf("error while reading\n");
        return ERR;
    }
    int md;
    printf("input max distance (A): ");
    if (scanf("%d", &md) != 1)
    {
        printf("error while reading\n");
        return ERR;
    }
    if (fscanf(f, "%zu", &n) != 1 || n < 1)
    {
        printf("error while reading\n");
        return ERR;
    }
    vector_t *e = alloc_vector(INIT_SIZE);
    read_vector(f, e, reader_elem, n);
    int cycle_vertex;
    struct timeval tv_start, tv_stop;
    gettimeofday(&tv_start, NULL);
    int *d = solve(e, n, v, &cycle_vertex);
    gettimeofday(&tv_stop, NULL);
    int64_t time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    if (cycle_vertex != -1)
        printf("there is negative cycle from %d!\n", cycle_vertex);
    for (size_t i = 1; i < n + 1; i++)
    {
        if (d[i] != INF)
        {
            printf("from %zu to %zu distance is %d", v, i, d[i]);
            if (d[i] <= md)
                printf(" <- less than or equal to max distance");
            printf("\n");
        }
        else
            printf("from %zu to %zu distance is inf\n", v, i);
    }
    printf("time: %lldms\n", time);
    printf("graph size: %lu\nanswer array size: %lu\n", sizeof(elem_t) * e->size + sizeof(vector_t), sizeof(int) * n);
    to_dot("viz/graph.gv", e);
    system("dot -Tpng viz/graph.gv -o viz/graph.png && open viz/graph.png");
    free(d);
    free_vector(e, free_elem);
    fclose(f);
    return 0;
}
