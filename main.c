#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define OK 0
#define ERR 1
#define ERR_LEN -1
#define EMPTY -3
#define MAXLEN 40
typedef char string_t[MAXLEN];
typedef struct
{
    char mant_sign;
    long mantissa[MAXLEN];
    char deg_sign;
    long deg;
} number;
long read_str(FILE **f, string_t s, size_t max)
{
    size_t i = 0;
    int c = fgetc(*f);
    while (c == '\n')
        c = fgetc(*f);
    while (c != '\n' && c != EOF)
    {
        if (i < max - 1)
            s[i++] = c;
        else
            return ERR_LEN;
        c = fgetc(*f);
    }
    s[i] = '\0';
    if (i == 0)
        return EMPTY;
    return i;
}

int str_into_num(string_t s, number *n)
{
    n->mant_sign = '+';
    n->deg_sign = '+';
    short i = 0, is_dot = 0, is_e = 0, p = 0, not_zero = 0;
    if (isdigit(*s))
    {
        if (*s != '0')
        {
            n->mantissa[i++] = (long)(*s) - '0';
            not_zero = 1;
        }
    }
    else if ((*s) == '.')
        is_dot = 1;
    else if (*s == '+' || *s == '-')
        n->mant_sign = *s;
    else
        return ERR;
    s++;
    while(*s != '\0')
    {
        if (*s == '+' || *s == '-')
        {
            if (is_e)
                n->deg_sign = *s;
            else
                return ERR;
        }
        else if (isdigit(*s))
        {
            if (is_e)
            {
                n->deg = strtol(s, NULL, 10);
                break;
            }
            else
            {
                if (not_zero || *s != '0')
                {
                    n->mantissa[i++] = (long)(*s) - '0';
                    not_zero = 1;
                }
                if (is_dot)
                    p++;
            }
        }
        else if ((*s) == '.')
            is_dot = 1;
        else if ((*s) == 'E' || (*s) == 'e')
            is_e = 1;
        else
            return ERR;
        s++;
    }
    if (n->deg_sign == '-')
        n->deg += p;
    else
        n->deg -= p;
//    printf("%c", n->mant_sign);
//    for (int k = 0; k < i; k++)
//        printf("%ld", n->mantissa[k]);
//    printf("%c", n->deg_sign);
//    printf("%ld", n->deg);
    return OK;
}
int minus(long a[MAXLEN], long b[MAXLEN], long r[MAXLEN])
{
    
    return OK;
}
int divide(number *a, number *b, number *r)
{
    return OK;
}
void print_num(number *a)
{
    
}
int main(int argc, char **argv)
{
    string_t s;
    number a, b, r;
    if (read_str(&stdin, s, MAXLEN) < 1)
        return ERR;
    str_into_num(s, &a);
    if (read_str(&stdin, s, MAXLEN) < 1)
        return ERR;
    str_into_num(s, &b);
    divide(&a, &b, &r);
//    print_num(&r);
    return OK;
}
