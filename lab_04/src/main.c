#include <stdio.h>
#include <stdlib.h>
#include "element.h"
#include "stack_on_array.h"
#include "stack_on_list.h"
#define OK                   0
#define ERR                  1
#define ERR_LEN              -1
#define ERR_READ             2
#define MAX_COUNT            100
#define MAXLEN               300
#define EQUAL                1
#define NOT_EQUAL            0

#define EXIT                                 0
#define PUSH_TO_STACK_ON_ARRAY               1
#define POP_FROM_STACK_ON_ARRAY              2
#define CHECK_EXPRESSION_WITH_STACK_ON_ARRAY 3
#define PUSH_TO_STACK_ON_LIST                4
#define POP_FROM_STACK_ON_LIST               5
#define CHECK_EXPRESSION_WITH_STACK_ON_LIST  6
#define STATISTICS                           7
int is_inverse(char scope_lhs, char scope_rhs)
{
    if (scope_lhs == '[' && scope_rhs == ']')
        return EQUAL;
    if (scope_lhs == '(' && scope_rhs == ')')
        return EQUAL;
    if (scope_lhs == '{' && scope_rhs == '}')
        return EQUAL;
    return NOT_EQUAL;
}
int is_right_position_of_scopes(FILE *f, void *stack, int (*push)(void *, void *), int (*pop)(void *, void *), void (*print) (void *))
{
    rewind(f);
    int rv = 1;
    char value, pop_value;
    for (int c = fgetc(f); c != '\n' && c != EOF; c = fgetc(f))
    {
        value = c;
        if (value == '{' || value == '[' || value == '(')
        {
            push(stack, &value);
            print(stack);
        }
        if (value == '}' || value == ']' || value == ')')
        {
            pop(stack, &pop_value);
            if (!is_inverse(pop_value, value))
                rv = 0;
            print(stack);
        }
    }
    return rv;
}
int main(void)
{
    FILE *in_stream = fopen("file.txt", "r");
    stack_on_list_t sl;
    stack_on_array_t sa;
    init_stack_on_list(&sl, MAX_COUNT);
    init_stack_on_array(&sa, MAX_COUNT);
    if (is_right_position_of_scopes(in_stream, &sl, push_to_stack_on_list, pop_from_stack_on_list, print_stack_on_list))
        printf("correct\n");
    else
        printf("incorrect\n");
    
    if (is_right_position_of_scopes(in_stream, &sa, push_to_stack_on_array, pop_from_stack_on_array, print_stack_on_array))
        printf("correct\n");
    else
        printf("incorrect\n");
    fclose(in_stream);
    delete_stack_on_list(&sl);
    delete_stack_on_array(&sa);
    return OK;
}
