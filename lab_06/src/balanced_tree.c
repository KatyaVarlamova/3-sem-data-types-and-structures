#include "balanced_tree.h"
balanced_tree_node_t * create_balanced_tree_node(elem_t *elem)
{
    balanced_tree_node_t *node = malloc(sizeof(balanced_tree_node_t));
    if (node)
    {
        node->data = elem;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
    }
    return node;
}
void delete_balanced_tree_node(balanced_tree_node_t **node, void (* free_elem)(void *val))
{
    if (node == NULL)
        return;
    if (*node != NULL && free_elem != NULL)
        free_elem((*node)->data);
    free(*node);
    *node = NULL;
}
balanced_tree_node_t * read_balanced_tree(char *filename, void *(*reader)(FILE *f), int (* comp)(const void *lhs, const void *rhs))
{
    FILE *f = fopen(filename, "r");
    void *el = reader(f);
    if (el == NULL)
        return NULL;
    balanced_tree_node_t *root = create_balanced_tree_node(el), *node;
    while ((el = reader(f)) != NULL)
    {
        int added = 0;
        node = create_balanced_tree_node(el);
        root = insert_balanced_tree_node(root, node, comp, &added);
        if (added == 1)
            delete_balanced_tree_node(&node, free_elem);
    }
    fclose(f);
    return root;
}
void print_balanced_tree(FILE *f, balanced_tree_node_t *tree, void print_elem(FILE *f, const void *elem))
{
    if (tree == NULL)
        return;
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
    print_balanced_tree(f, tree->left, print_elem);
    print_balanced_tree(f, tree->right, print_elem);
}
void free_balanced_tree(balanced_tree_node_t *tree, void (* free_elem)(void *val))
{
    if (tree == NULL)
        return;
    balanced_tree_node_t *l = tree->left, *r = tree->right;
    delete_balanced_tree_node(&tree, free_elem);
    free_balanced_tree(l, free_elem);
    free_balanced_tree(r, free_elem);
}
short get_height(balanced_tree_node_t *p)
{
    return p ? p->height : 0;
}
void update_height(balanced_tree_node_t *p)
{
    p->height = (get_height(p->left) > get_height(p->right) ? get_height(p->left) : get_height(p->right)) + 1;
}
static balanced_tree_node_t * rotate_left(balanced_tree_node_t *node)
{
    balanced_tree_node_t *right = node->right;
    node->right = right->left;
    right->left = node;
    update_height(node);
    update_height(right);
    return right;
}
static balanced_tree_node_t * rotate_right(balanced_tree_node_t *node)
{
    balanced_tree_node_t *left = node->left;
    node->left = left->right;
    left->right = node;
    update_height(node);
    update_height(left);
    return left;
}
int b_factor(balanced_tree_node_t *node)
{
    return get_height(node->right) - get_height(node->left);
}
static balanced_tree_node_t * balance(balanced_tree_node_t *node)
{
    update_height(node);
    if (b_factor(node) == 2)
    {
        if (b_factor(node->right) < 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    if (b_factor(node) == -2)
    {
        if (b_factor(node->left) > 0)
            node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    return node;
}
balanced_tree_node_t * insert_balanced_tree_node(balanced_tree_node_t *tree, balanced_tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), int *added)
{
    if (tree == NULL)
        return node;
    int cmp = comp(tree->data, node->data);
    if (cmp > 0)
        tree->left = insert_balanced_tree_node(tree->left, node, comp, added);
    else if (cmp < 0)
        tree->right = insert_balanced_tree_node(tree->right, node, comp, added);
    else
        *added = 1;
    return balance(tree);
}
static balanced_tree_node_t *find_min(balanced_tree_node_t *tree)
{
    for (; tree->left; tree = tree->left);
    return tree;
}
static balanced_tree_node_t *delete_min(balanced_tree_node_t *tree)
{
    if (tree->left == NULL)
        return tree->right;
    tree->left = delete_min(tree->left);
    return balance(tree);
}
int count_elems_btree(balanced_tree_node_t *btree)
{
    if (btree == NULL)
        return 0;
    int count = 0;
    count += count_elems_btree(btree->left);
    count += count_elems_btree(btree->right);
    count++;
    return count;
}
balanced_tree_node_t * remove_balanced_tree_node(balanced_tree_node_t *tree, const balanced_tree_node_t *node, int (* comp)(const void *lhs, const void *rhs), void (* free_elem)(void *val), stat_values_t *stat)
{
    if (tree == NULL)
        return NULL;
    stat->comp_count++;
    int cmp = comp(tree->data, node->data);
    if (cmp > 0)
        tree->left = remove_balanced_tree_node(tree->left, node, comp, free_elem, stat);
    else if (cmp < 0)
        tree->right = remove_balanced_tree_node(tree->right, node, comp, free_elem, stat);
    else
    {
        balanced_tree_node_t *l = tree->left;
        balanced_tree_node_t *r = tree->right;
        delete_balanced_tree_node(&tree, free_elem);
        if (r == NULL)
            return l;
        balanced_tree_node_t *min = find_min(r);
        if (min != r)
            min->right = delete_min(r);
        min->left = l;
        return balance(min);
    }
    return balance(tree);
}
void btree_to_dot(char *name,balanced_tree_node_t *tree, void print_elem(FILE *f, const void *elem))
{
    FILE *f = fopen(name ,"w");
    fprintf(f, "digraph test_tree {\n");
    if (tree && tree->left == NULL && tree->right == NULL)
        print_elem(f, tree->data);
    else
        print_balanced_tree(f, tree, print_elem);
    fprintf(f, "}\n");
    fclose(f);
}
