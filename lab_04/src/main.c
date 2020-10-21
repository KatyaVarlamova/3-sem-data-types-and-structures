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
#define MAX_COUNT            100
#define MAXLEN               300
#define YES                  1
#define NO                   0

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
int is_right_position_of_scopes(FILE *f, void *stack, int (*push)(void *, void *), int (*pop)(void *, void *), void (*print) (void *), int is_to_print)
{
    rewind(f);
    int rv = YES;
    char value, pop_value;
    for (int c = fgetc(f); c != '\n' && c != EOF; c = fgetc(f))
    {
        value = c;
        if (value == '{' || value == '[' || value == '(')
        {
            push(stack, &value);
            if (is_to_print)
                print(stack);
        }
        if (value == '}' || value == ']' || value == ')')
        {
            pop(stack, &pop_value);
            if (!is_inverse(pop_value, value))
                rv = NO;
            if (is_to_print)
                print(stack);
        }
    }
    return rv;
}
void check_expression_arr(FILE *in_stream, int is_to_print)
{
    stack_on_array_t sa;
    init_stack_on_array(&sa, MAX_COUNT);
    if (is_right_position_of_scopes(in_stream, &sa, push_to_stack_on_array, pop_from_stack_on_array, print_stack_on_array, is_to_print) == YES)
    {
        if (is_to_print)
            printf("correct");
    }
    else if (is_to_print)
        printf("incorrect");
    delete_stack_on_array(&sa);
}
void check_expression_list(FILE *in_stream, int is_to_print)
{
    stack_on_list_t sl;
    init_stack_on_list(&sl, MAX_COUNT);
    if (is_right_position_of_scopes(in_stream, &sl,push_to_stack_on_list, pop_from_stack_on_list, print_stack_on_list, is_to_print) == YES)
    {
        if (is_to_print)
            printf("correct");
    }
    else if (is_to_print)
        printf("incorrect");
    delete_stack_on_list(&sl);
}
void push(void *stack, int (*push)(void *, void *))
{
    int c = getc(stdin);
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
void statistics(FILE *in_stream)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    printf("x--------------- x---------------x---------------x\n");
    printf("|     stack      |     time      |     memory    |\n");
    printf("x----------------x---------------x---------------x\n");
    printf("| on linked list |");
    for (size_t j = 0; j < 1000; j++)
    {
        gettimeofday(&tv_start, NULL);
        check_expression_list(in_stream, NO);
        gettimeofday(&tv_stop, NULL);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    printf("%15.2lf|", time / 1000.0);
    printf("%15zu|\n", sizeof(stack_on_list_t));
    printf("x----------------x---------------x---------------x\n");
    printf("|   on vector    |");
    time = 0;
    for (size_t j = 0; j < 1000; j++)
    {
        gettimeofday(&tv_start, NULL);
        check_expression_arr(in_stream, NO);
        gettimeofday(&tv_stop, NULL);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    printf("%15.2lf|", time / 1000.0);
    printf("%15zu|\n", sizeof(stack_on_array_t));
    printf("x----------------x---------------x---------------x\n");
    
}
int main(void)
{
    FILE *in_stream = fopen("file.txt", "r");
    int choise;
    stack_on_list_t sl;
    stack_on_array_t sa;
    init_stack_on_list(&sl, MAX_COUNT);
    init_stack_on_array(&sa, MAX_COUNT);
    // print_menu();
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
                check_expression_arr(in_stream, YES);
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
                check_expression_list(in_stream, YES);
                break;
            case STATISTICS:
                statistics(in_stream);
                break;
            default:
                printf("there is no such action\n");
        }
        //print_menu();
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
