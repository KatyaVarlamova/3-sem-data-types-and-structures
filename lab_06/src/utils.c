#include "utils.h"
#define COUNT 500
void stat_file(char *filename, elem_t *elem)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    size_t size = 0;
    int comp_av = 0;
    stat_values_t stat;
    for (int i = 0; i < 1; i++)
    {
        stat.comp_count = 0;
        stat.elems_count = 0;
        char *copy_name = make_file_copy(filename, reader_elem, print_elem);
        gettimeofday(&tv_start, NULL);
        delete_from_file(copy_name, elem, reader_elem, print_elem, compare_elems, &stat);
        gettimeofday(&tv_stop, NULL);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        free_filename(copy_name);
        comp_av += stat.comp_count;
    }
    size = 0;
    printf("%15.2lf|", (1.0 * time));
    printf("%15zu|", size);
    printf("%15.2lf|\n", (1.0 * comp_av));
}
void stat_hash_table(char *filename, elem_t *elem, size_t size_table, double conflicts)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    size_t size = 0;
    int comp_av = 0;
    stat_values_t stat;
    char *copy = make_file_copy(filename, reader_elem, print_elem);
    FILE *f = fopen(copy, "r");
    
    hash_table_t *table = create_hash_table(size_table, hash_func_sum);
    read_hash_table(filename, table, reader_elem, &stat);
    table = reduce_conflicts_by_restruction_ht(table, conflicts);
    
    int read = 0;
    if (elem == NULL)
        read = 1;
    for (int i = 0; i < COUNT; i++)
    {
        stat.elems_count = 0;
        stat.comp_count = 0;
        if (read)
        {
            elem = reader_elem(f);
            if (elem == NULL)
            {
                rewind(f);
                elem = reader_elem(f);
                if (elem == NULL)
                    break;
            }
        }
        read_hash_table(filename, table, reader_elem, &stat);
        gettimeofday(&tv_start, NULL);
        delete_hash_table_elem(table, elem, free_elem, compare_elems, &stat);
        gettimeofday(&tv_stop, NULL);
        for (size_t i = 0; i < table->size; i++)
            free_list(&(table->data[i].head), free_elem);
        if (read)
            free_elem(elem);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        comp_av += stat.comp_count;
    }
    fclose(f);
    free_filename(copy);
    size = stat.elems_count * sizeof(node_t) + table->size * sizeof(list_t) + sizeof(hash_table_t);
    free_hash_table(&table, free_elem);;
    printf("%15.2lf|", (1.0 * time) / COUNT);
    printf("%15zu|", size);
    printf("%15.2lf|\n", (1.0 * comp_av) / COUNT);
}
void stat_bt(char *filename, elem_t *elem)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    size_t size = 0;
    int comp_av = 0;
    stat_values_t stat;
    tree_node_t n = {elem, NULL, NULL};
    char *copy = make_file_copy(filename, reader_elem, print_elem);
    FILE *f = fopen(copy, "r");
    int read = 0;
    if (elem == NULL)
        read = 1;
    for (int i = 0; i < COUNT; i++)
    {
        stat.comp_count = 0;
        tree_node_t *tree_root = read_tree(filename, reader_elem, compare_elems);
        stat.elems_count = count_elems_tree(tree_root);
        if (read)
        {
            elem = reader_elem(f);
            n.data = elem;
            if (elem == NULL)
            {
                rewind(f);
                elem = reader_elem(f);
                n.data = elem;
                if (elem == NULL)
                    break;
            }
        }
        gettimeofday(&tv_start, NULL);
        tree_root = remove_tree_node(tree_root, &n, compare_elems, free_elem, &stat);
        gettimeofday(&tv_stop, NULL);
        if (read)
            free_elem(elem);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        free_tree(tree_root, free_elem);
        comp_av += stat.comp_count;
    }
    fclose(f);
    free_filename(copy);
    size = stat.elems_count * sizeof(tree_node_t);
    printf("%15.2lf|", (1.0 * time) / COUNT);
    printf("%15zu|", size);
    printf("%15.2lf|\n", (1.0 * comp_av) / COUNT);
}
void stat_bbt(char *filename, elem_t *elem)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    size_t size = 0;
    int comp_av = 0;
    stat_values_t stat;
    balanced_tree_node_t bn = {elem, NULL, NULL};
    char *copy = make_file_copy(filename, reader_elem, print_elem);
    FILE *f = fopen(copy, "r");
    int read = 0;
    if (elem == NULL)
        read = 1;
    for (int i = 0; i < COUNT; i++)
    {
        stat.comp_count = 0;
        balanced_tree_node_t *btree_root = read_balanced_tree(filename, reader_elem, compare_elems);
        stat.elems_count = count_elems_btree(btree_root);
        if (read)
        {
            elem = reader_elem(f);
            bn.data = elem;
            if (elem == NULL)
            {
                rewind(f);
                elem = reader_elem(f);
                bn.data = elem;
                if (elem == NULL)
                    break;
            }
        }
        gettimeofday(&tv_start, NULL);
        btree_root = remove_balanced_tree_node(btree_root, &bn, compare_elems, free_elem, &stat);
        gettimeofday(&tv_stop, NULL);
        if (read)
            free_elem(elem);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        free_balanced_tree(btree_root, free_elem);
        comp_av += stat.comp_count;
    }
    fclose(f);
    free_filename(copy);
    size = stat.elems_count * sizeof(balanced_tree_node_t);
    printf("%15.2lf|", (1.0 * time) / COUNT);
    printf("%15zu|", size);
    printf("%15.2lf|\n", (1.0 * comp_av) / COUNT);
}
void statistics(char *filename)
{
    printf("input size of table: ");
    long size_table;
    if (scanf("%ld", &size_table) != 1 || size_table <= 0)
        return;
    printf("input max average conflicts in table: ");
    double conflicts;
    if (scanf("%lf", &conflicts) != 1 || conflicts < 1)
        return;
    printf("input elem: ");
    elem_t *elem = reader_elem(stdin); 
    printf("delete operation of element:\n");
    printf("x--------------- x---------------x---------------x---------------x\n");
    printf("|  data struct   |     time      |     memory    |  comparisons  |\n");
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|     file       |");
    stat_file(filename, elem);
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|   hash table   |");
    stat_hash_table(filename, elem, size_table, conflicts);
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|binary tree (BT)|");
    stat_bt(filename, elem);
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|  balanced BT   |");
    stat_bbt(filename, elem);
    printf("x----------------x---------------x---------------x---------------x\n");
    
    printf("delete operation average:\n");
    printf("x--------------- x---------------x---------------x---------------x\n");
    printf("|  data struct   |     time      |     memory    |  comparisons  |\n");
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|     file       |");
    stat_file(filename, elem);
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|   hash table   |");
    stat_hash_table(filename, NULL, size_table, conflicts);
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|binary tree (BT)|");
    stat_bt(filename, NULL);
    printf("x----------------x---------------x---------------x---------------x\n");
    printf("|  balanced BT   |");
    stat_bbt(filename, NULL);
    printf("x----------------x---------------x---------------x---------------x\n");
    free_elem(elem);
}
