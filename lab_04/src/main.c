#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "element.h"
#include "stack_on_array.h"
#include "stack_on_list.h"

#define OK                   0
#define ERR                  1
#define ERR_LEN              -1
#define ERR_READ             2
#define MAXLEN               300
#define YES                  1
#define NO                   0

#define RIRHT                0
#define COUNT_ERR            1
#define OVERFLOW_ERR         2
#define EMPTY_ERR            3
#define MEM_ERR              4

#define EXIT                                 0
#define PUSH_TO_STACK_ON_ARRAY               1
#define POP_FROM_STACK_ON_ARRAY              2
#define PRINT_STACK_ON_ARRAY                 3
#define CHECK_EXPRESSION_WITH_STACK_ON_ARRAY 4
#define PUSH_TO_STACK_ON_LIST                5
#define POP_FROM_STACK_ON_LIST               6
#define PRINT_STACK_ON_LIST                  7
#define CHECK_EXPRESSION_WITH_STACK_ON_LIST  8
#define STATISTICS                           9
int is_inverse(char scope_lhs, char scope_rhs)
{
    if (scope_lhs == '[' && scope_rhs == ']')
        return YES;
    if (scope_lhs == '(' && scope_rhs == ')')
        return YES;
    if (scope_lhs == '{' && scope_rhs == '}')
        return YES;
    return NO;
}
int is_right_position_of_scopes(FILE *f, void *stack, int (*push)(void *, void *), int (*pop)(void *, void *), void (*print) (void *), int is_to_print, size_t *count)
{
    rewind(f);
    int r;
    elem_t value, pop_value;
    *count = 0;
    size_t k = 0;
    for (int c = fgetc(f); c != '\n' && c != EOF; c = fgetc(f))
    {
        value = c;
        if (value == '{' || value == '[' || value == '(')
        {
            r = push(stack, &value);
            if (r == LENGTH_ERROR)
                return OVERFLOW_ERR;
            else if (r != OK)
                return MEM_ERR;
            k++;
            if (k > *count)
                *count = k;
            if (is_to_print)
                print(stack);
        }
        if (value == '}' || value == ']' || value == ')')
        {
            r = pop(stack, &pop_value);
            if (r == LENGTH_ERROR)
                return EMPTY_ERR;
            else if (r != OK)
                return MEM_ERR;
            k--;
            if (!is_inverse(pop_value, value))
                return COUNT_ERR;
            if (is_to_print)
                print(stack);
        }
    }
    if (k != 0)
        return EMPTY_ERR;
    return RIRHT;
}
int check_expression_arr(FILE *in_stream, int is_to_print, size_t *size, size_t max_count)
{
    stack_on_array_t sa;
    init_stack_on_array(&sa, max_count);
    size_t max_size;
    int rv;
    if ((rv = is_right_position_of_scopes(in_stream, &sa, push_to_stack_on_array, pop_from_stack_on_array, print_stack_on_array, is_to_print, &max_size)) == RIRHT)
    {
        if (is_to_print)
            printf("correct\n");
    }
    else
    {
        if (is_to_print)
        {
            printf("incorrect");
            if (rv == OVERFLOW_ERR)
                printf(": stack overflow");
            else if (rv == MEM_ERR)
                printf(": memory error");
            else if (rv == EMPTY_ERR)
                printf(": not enough brackets");
            printf("\n");
        }
    }
    *size = sa.data.alloc_size * sa.size_of_element + sizeof(sa);
    delete_stack_on_array(&sa);
    return rv;
}
int check_expression_list(FILE *in_stream, int is_to_print, size_t *size, size_t max_count)
{
    stack_on_list_t sl;
    init_stack_on_list(&sl, max_count);
    size_t max_size;
    int rv;
    if ((rv = is_right_position_of_scopes(in_stream, &sl,push_to_stack_on_list, pop_from_stack_on_list, print_stack_on_list, is_to_print, &max_size)) == RIRHT)
    {
        if (is_to_print)
            printf("correct\n");
    }
    else
    {
        if (is_to_print)
        {
            printf("incorrect");
            if (rv == OVERFLOW_ERR)
                printf(": stack overflow");
            else if (rv == MEM_ERR)
                printf(": memory error");
            else if (rv == EMPTY_ERR)
                printf(": not enough brackets");
            printf("\n");
        }
    }
    *size = max_size * sizeof(node_t) + sizeof(sl) - sizeof(sl.adrs);
    delete_stack_on_list(&sl);
    return rv;
}
void statistics(FILE *in_stream, size_t max_count)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    size_t size = 0;
    printf("x--------------- x---------------x---------------x\n");
    printf("|     stack      |     time      |     memory    |\n");
    printf("x----------------x---------------x---------------x\n");
    printf("| on linked list |");
    int rv;
    if ((rv = check_expression_list(in_stream, NO, &size, max_count)) != RIRHT)
    {
        printf("%15s|", "-");
        printf("%15s|", "-");
        printf(" incorrect");
        if (rv == OVERFLOW_ERR)
            printf(": stack overflow");
        else if (rv == MEM_ERR)
            printf(": memory error");
        else if (rv == EMPTY_ERR)
            printf(": not enough brackets");
        printf("\n");
    }
    else
    {
        for (size_t j = 0; j < 10; j++)
        {
            gettimeofday(&tv_start, NULL);
            check_expression_list(in_stream, NO, &size, max_count);
            gettimeofday(&tv_stop, NULL);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        printf("%15.2lf|", time / 10.0);
        printf("%15zu|\n", size);
    }
    printf("x----------------x---------------x---------------x\n");
    printf("|   on vector    |");
    size = 0;
    time = 0;
    if ((rv = check_expression_arr(in_stream, NO, &size, max_count)) != RIRHT)
    {
        printf("%15s|", "-");
        printf("%15s|", "-");
        printf(" incorrect");
        if (rv == OVERFLOW_ERR)
            printf(": stack overflow");
        else if (rv == MEM_ERR)
            printf(": memory error");
        else if (rv == EMPTY_ERR)
            printf(": not enough brackets");
        printf("\n");
    }
    else
    {
        for (size_t j = 0; j < 10; j++)
        {
            gettimeofday(&tv_start, NULL);
            check_expression_arr(in_stream, NO, &size, max_count);
            gettimeofday(&tv_stop, NULL);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        printf("%15.2lf|", time / 10.0);
        printf("%15zu|\n", size);
    }
    printf("x----------------x---------------x---------------x\n");
    
}
void push(void *stack, int (*push)(void *, void *))
{
    printf("symbol: ");
    long c = getc(stdin);
    while (c == '\n' || c == ' ')
        c = getc(stdin);
    int rv = push(stack, &c);
    if (rv == LENGTH_ERROR)
        printf("impossible to push because of stack overflow\n");
    else if (rv != OK)
        printf("errors with memory\n");
}
void pop(void *stack, int (*pop)(void *, void *))
{
    char c;
    int rv = pop(stack, &c);
    if (rv == LENGTH_ERROR)
        printf("impossible to pop because stack is empty\n");
    else if (rv == OK)
        printf("extracted value: %c\n", c);
    else
        printf("errors with memory\n");
}
void print(void *stack, void (*print)(void *))
{
    print(stack);
}
void print_menu()
{
    printf(
        "EXIT                                 0\n"
        "PUSH TO STACK ON ARRAY               1\n"
        "POP FROM STACK ON ARRAY              2\n"
        "PRINT STACK ON ARRAY                 3\n"
        "CHECK EXPRESSION WITH STACK ON ARRAY 4\n"
        "PUSH TO STACK ON LIST                5\n"
        "POP FROM STACK ON LIST               6\n"
        "PRINT STACK ON LIST                  7\n"
        "CHECK EXPRESSION WITH STACK ON LIST  8\n"
        "STATISTICS                           9\n");
}
int main(int argc, char **argv)
{
    printf("Program shows working of stacks based on dynamic array and list. It's possible to push to, pop from and print stacks.\n");
    printf("Also, this program can read expression from file (name should be given in command line args) and determine if brackets in expression are put in correct way. This function works with both stacks: based on list and based on array. Compare efficiency of stacks is possible in statistics.\n");
    FILE *in_stream;
    if (argc < 2 || (in_stream = fopen(argv[1], "r")) == NULL)
    {
        printf("error while opening file\n");
        return ERR_READ;
    }
    size_t max_count;
    printf("enter max size of stack: ");
    if (scanf("%zu", &max_count) != 1 || max_count < 1)
    {
        printf("error while reading max size\n");
        return ERR_READ;
    }
    int choise;
    size_t size;
    stack_on_list_t sl;
    stack_on_array_t sa;
    init_stack_on_list(&sl, max_count);
    init_stack_on_array(&sa, max_count);
    print_menu();
    printf("action: ");
    if (scanf("%d", &choise) != 1)
    {
        printf("errors while reading\n");
        return ERR_READ;
    }
    while (choise != EXIT)
    {
        switch (choise)
        {
            case PUSH_TO_STACK_ON_ARRAY:
                push(&sa, push_to_stack_on_array);
                break;
            case POP_FROM_STACK_ON_ARRAY:
                pop(&sa, pop_from_stack_on_array);
                break;
            case PRINT_STACK_ON_ARRAY:
                print(&sa, print_stack_on_array);
                break;
            case CHECK_EXPRESSION_WITH_STACK_ON_ARRAY:
                check_expression_arr(in_stream, YES, &size, max_count);
                break;
            case PUSH_TO_STACK_ON_LIST:
                push(&sl, push_to_stack_on_list);
                break;
            case POP_FROM_STACK_ON_LIST:
                pop(&sl, pop_from_stack_on_list);
                break;
            case PRINT_STACK_ON_LIST:
                print(&sl, print_stack_on_list);
                break;
            case CHECK_EXPRESSION_WITH_STACK_ON_LIST:
                check_expression_list(in_stream, YES, &size, max_count);
                break;
            case STATISTICS:
                statistics(in_stream, max_count);
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
    delete_stack_on_list(&sl);
    delete_stack_on_array(&sa);
    fclose(in_stream);
    return OK;
}
