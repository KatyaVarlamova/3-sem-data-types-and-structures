#include "tree.h"
tree_node_t * create_tree_node(elem_t *elem)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (node)
    {
        node->data = elem;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}
void delete_tree_node(tree_node_t **node, void (* free_elem)(void *val))
{
    if (node == NULL)
        return;
    if (*node != NULL && free_elem != NULL)
        free_elem((*node)->data);
    free(*node);
    *node = NULL;
}
tree_node_t * read_tree(char *filename, void *(*reader)(FILE *f), int (* comp)(const void *lhs, const void *rhs))
{
    FILE *f = fopen(filename, "r");
    void *el = reader(f);
    if (el == NULL)
        return NULL;
    tree_node_t *root = create_tree_node(el), *node;
    while ((el = reader(f)) != NULL)
    {
        int added = 0;
        node = create_tree_node(el);
        insert_tree_node(root, node, comp, &added);
        if (added == 1)
            delete_tree_node(&node, free_elem);
    }
    fclose(f);
    return root;
}
void print_tree(FILE *f, tree_node_t *tree, void print_elem(FILE *f, const void *elem))
{
    if (tree == NULL)
        return;
    print_tree(f, tree->left, print_elem);
    if (tree)
    {
        if (tree->left)
        {
            print_elem(f, tree->data);
            fprintf(f, " -> ");
            print_elem(f, tree->left->data);
            fprintf(f, "\n");
        }
        if (tree->right)
        {
            print_elem(f, tree->data);
            fprintf(f, " -> ");
            print_elem(f, tree->right->data);
            fprintf(f, "\n");
        }
    }
    print_tree(f, tree->right, print_elem);
}
int count_elems_tree(tree_node_t *tree)
{
    if (tree == NULL)
        return 0;
    int count = 0;
    count += count_elems_tree(tree->left);
    count += count_elems_tree(tree->right);
    count++;
    return count;
}
void free_tree(tree_node_t *tree, void (* free_elem)(void *val))
{
    if (tree == NULL)
        return;
    tree_node_t *l = tree->left, *r = tree->right;
    delete_tree_node(&tree, free_elem);
    free_tree(l, free_elem);
    free_tree(r, free_elem);
}
tree_node_t * insert_tree_node(tree_node_t *tree, tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), int *added)
{
    if (tree == NULL)
        return node;
    int cmp = comp(tree->data, node->data);
    if (cmp > 0)
        tree->left = insert_tree_node(tree->left, node, comp, added);
    else if (cmp < 0)
        tree->right = insert_tree_node(tree->right, node, comp, added);
    else
        *added = 1;
    return tree;
}
static tree_node_t *find_min(tree_node_t *tree)
{
    for (; tree->left; tree = tree->left);
    return tree;
    //return tree->left ? find_min(tree->left) : tree;
}
static tree_node_t *delete_min(tree_node_t *tree)
{
//    for (; tree->left; tree = tree->left);
    if (tree->left == NULL)
        return tree->right;
    tree->left = delete_min(tree->left);
    return tree;
}
tree_node_t * remove_tree_node(tree_node_t *tree, const tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), void (* free_elem)(void *val), stat_values_t *stat)
{
    if (tree == NULL)
        return NULL;
    stat->comp_count++;
    int cmp = comp(tree->data, node->data);
    if (cmp > 0)
        tree->left = remove_tree_node(tree->left, node, comp, free_elem, stat);
    else if (cmp < 0)
        tree->right = remove_tree_node(tree->right, node, comp, free_elem, stat);
    else
    {
        tree_node_t *l = tree->left;
        tree_node_t *r = tree->right;
        delete_tree_node(&tree, free_elem);
        if (r == NULL)
            return l;
        tree_node_t *min = find_min(r);
        if (min != r)
            min->right = delete_min(r);
        min->left = l;
        return min;
    }
    return tree;
}
void tree_to_dot(char *name, tree_node_t *tree, void print_elem(FILE *f, const void *elem))
{
    FILE *f = fopen(name ,"w");
    fprintf(f, "digraph test_tree {\n");
    if (tree && tree->left == NULL && tree->right == NULL)
        print_elem(f, tree->data);
    else
        print_tree(f, tree, print_elem);
    fprintf(f, "}\n");
    fclose(f);
}
