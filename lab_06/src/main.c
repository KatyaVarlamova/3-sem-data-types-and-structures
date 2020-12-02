#include "assert.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "balanced_tree.h"
#include "file.h"
#include "hash_table.h"
#include "tree.h"
#include "utils.h"

#define OK                   0
#define ERR_READ             2

#define EXIT                                 0
#define OUT_BINARY_TREE                      1
#define ADD_TO_BINARY_TREE                   2
#define DELETE_FROM_BINARY_TREE              3
#define OUT_BALANCED_BINARY_TREE             4
#define ADD_TO_BALANCED_BINARY_TREE          5
#define DELETE_FROM_BALANCED_BINARY_TREE     6
#define OUT_HASH_TABLE                       7
#define ADD_TO_HASH_TABLE                    8
#define DELETE_FROM_HASH_TABLE               9
#define RESTRUCT_HASH_TABLE                  10
#define OUT_COPY_OF_FILE                     11
#define DELETE_FROM_COPY_OF_FILE             12
#define STATISTICS                           13

void print_menu()
{
    printf(
        "EXIT                                 0\n"
        "OUT BINARY TREE                      1\n"
        "ADD TO BINARY TREE                   2\n"
        "DELETE FROM BINARY TREE              3\n"
        "OUT BALANCED BINARY TREE             4\n"
        "ADD TO BALANCED BINARY TREE          5\n"
        "DELETE FROM BALANCED BINARY TREE     6\n"
        "OUT HASH TABLE                       7\n"
        "ADD TO HASH TABLE                    8\n"
        "DELETE FROM HASH TABLE               9\n"
        "RESTRUCT HASH TABLE                  10\n"
        "OUT COPY OF FILE                     11\n"
        "DELETE FROM COPY OF FILE             12\n"
        "STATISTICS                           13\n");
}

int main(int argc, char **argv)
{
    FILE *in_stream;
    if (argc < 2 || (in_stream = fopen(argv[1], "r")) == NULL)
    {
        printf("error while opening file\n");
        return ERR_READ;
    }
    fclose(in_stream);
    int choise;
    print_menu();
    
    tree_node_t *tree_root = read_tree(argv[1], reader_elem, compare_elems);
    balanced_tree_node_t *btree_root = read_balanced_tree(argv[1], reader_elem, compare_elems);
    
    printf("input size of table: ");
    long size;
    if (scanf("%ld", &size) != 1 || size <= 0)
        return ERR_READ;
    hash_table_t *table = create_hash_table(size, hash_func_sum);
    stat_values_t stat;
    read_hash_table(argv[1], table, reader_elem, &stat);
    
    char *copy_name = make_file_copy(argv[1], reader_elem, print_elem);
    
    printf("action: ");
    if (scanf("%d", &choise) != 1)
    {
        printf("errors while reading\n");
        return ERR_READ;
    }
    while (choise != EXIT)
    {
        switch (choise) {
            case OUT_BINARY_TREE:
            {
                print_tree(stdout, tree_root, print_elem);
                tree_to_dot("viz/graph.gv", tree_root, print_elem);
                system("dot -Tpng viz/graph.gv -o viz/graph.png && open viz/graph.png");
                break;
            }
            case ADD_TO_BINARY_TREE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                tree_node_t *n = create_tree_node(elem);
                int added = 0;
                tree_root = insert_tree_node(tree_root, n, compare_elems, &added);
                if (added)
                {
                    printf("elem has been already added\n");
                    delete_tree_node(&n, free_elem);
                }
                break;
            }
                case DELETE_FROM_BINARY_TREE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                tree_node_t n = {elem, NULL, NULL};
                tree_root = remove_tree_node(tree_root, &n, compare_elems, free_elem, &stat);
                free_elem(elem);
                break;
            }
                case OUT_BALANCED_BINARY_TREE:
            {
                print_balanced_tree(stdout, btree_root, print_elem);
                btree_to_dot("viz/bgraph.gv", btree_root, print_elem);
                system("dot -Tpng viz/bgraph.gv -o viz/bgraph.png && open viz/bgraph.png");
                break;
            }
            case ADD_TO_BALANCED_BINARY_TREE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                balanced_tree_node_t *n = create_balanced_tree_node(elem);
                int added = 0;
                btree_root = insert_balanced_tree_node(btree_root, n, compare_elems, &added);
                if (added)
                {
                    printf("elem has been already added\n");
                    delete_balanced_tree_node(&n, free_elem);
                }
                break;
            }
            case DELETE_FROM_BALANCED_BINARY_TREE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                balanced_tree_node_t n = {elem, NULL, NULL};
                btree_root = remove_balanced_tree_node(btree_root, &n, compare_elems, free_elem, &stat);
                free_elem(elem);
                break;
            }
            case OUT_HASH_TABLE:
            {
                print_hash_table(stdout, table, print_elem);
                break;
            }
            case ADD_TO_HASH_TABLE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                insert_hash_table_elem(table, elem);
                break;
            }
            case DELETE_FROM_HASH_TABLE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                delete_hash_table_elem(table, elem, free_elem, compare_elems, &stat);
                free_elem(elem);
                break;
            }
            case RESTRUCT_HASH_TABLE:
            {
                printf("please, choose hash func:\n- sum -> 1\n- xor -> 2\n");
                int ch;
                if (scanf("%d", &ch) != 1)
                    printf("read error\n");
                if (ch == 1)
                {
                    if (table->hash_func_type != hash_func_sum)
                        table = restruct(table, hash_func_sum);
                    else
                        printf("already was chosen this type of hash func\n");
                }
                else if (ch == 2)
                {
                    if (table->hash_func_type != hash_func_xor)
                        table = restruct(table, hash_func_xor);
                    else
                        printf("already was chosen this type of hash func\n");
                }
                else
                    printf("incorrect choise\n");
                break;
            }
            case OUT_COPY_OF_FILE:
            {
                print_file(stdout, copy_name, reader_elem, print_elem);
                break;
            }
            case DELETE_FROM_COPY_OF_FILE:
            {
                printf("input elem: ");
                elem_t *elem = reader_elem(stdin);
                delete_from_file(copy_name, elem, reader_elem, print_elem, compare_elems, &stat);
                free_elem(elem);
                break;
            }
            case STATISTICS:
            {
                statistics(argv[1]);
                break;
            }
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
    free_tree(tree_root, free_elem);
    free_balanced_tree(btree_root, free_elem);
    free_hash_table(&table, free_elem);
    free_filename(copy_name);
    return OK;
}
//    int choise;
//    print_menu();
//    printf("action: ");
//    if (scanf("%d", &choise) != 1)
//    {
//        printf("errors while reading\n");
//        return ERR_READ;
//    }

//balanced_tree_node_t *root = read_balanced_tree(in_stream, reader_elem, compare_elems);
//print_balanced_tree(stdout, root, print_elem);
//elem_t elem = {7};
//balanced_tree_node_t n = {&elem, NULL, NULL};
//root = remove_balanced_tree_node(root, &n, compare_elems, free_elem);
//printf("\n\n");
//print_balanced_tree(stdout, root, print_elem);
//
//elem_t elem2 = {8};
//balanced_tree_node_t n2 = {&elem2, NULL, NULL};
//root = remove_balanced_tree_node(root, &n2, compare_elems, free_elem);
//printf("\n");
//print_balanced_tree(stdout, root, print_elem);
//
//elem_t elem3 = {13};
//balanced_tree_node_t n3 = {&elem3, NULL, NULL};
//root = remove_balanced_tree_node(root, &n3, compare_elems, free_elem);
//printf("\n");
//print_balanced_tree(stdout, root, print_elem);
//
//free_balanced_tree(root, free_elem);

//tree_node_t *root = read_tree(in_stream, reader_elem, compare_elems);
//print_tree(stdout, root, print_elem);
//elem_t elem = {7};
//tree_node_t n = {&elem, NULL, NULL};
//root = remove_tree_node(root, &n, compare_elems, free_elem);
//printf("\n");
//print_tree(stdout, root, print_elem);
//
//elem_t elem2 = {11};
//tree_node_t n2 = {&elem2, NULL, NULL};
//root = remove_tree_node(root, &n2, compare_elems, free_elem);
//printf("\n");
//print_tree(stdout, root, print_elem);
//
//elem_t elem3 = {5};
//tree_node_t n3 = {&elem3, NULL, NULL};
//root = remove_tree_node(root, &n3, compare_elems, free_elem);
//printf("\n");
//print_tree(stdout, root, print_elem);
//
//free_tree(root, free_elem);

//hash_table_t *table = create_hash_table(7);
//read_hash_table(in_stream, table, reader_elem);
//elem_t a = {1};
//remove_hash_table_elem(table, &a, free_elem, compare_elems);
//print_hash_table(stdout, table, print_elem);
//free_hash_table(&table, free_elem);

//vector_t *vector = alloc_vector(5);
//read_vector(in_stream, vector, reader_elem);
//elem_t a = {10};
//delete_elem_in_vector(vector, &a, compare_elems, free_elem);
//print_vector(stdout, vector, print_elem);
//free_vector(vector, free_elem);


//const char *copy = make_file_copy(argv[1], reader_elem, print_elem);
//print_file(stdout, copy, reader_elem, print_elem);
//elem_t elem = {2};
//delete_from_file(copy, &elem, reader_elem, print_elem, compare_elems);
//print_file(stdout, copy, reader_elem, print_elem);
