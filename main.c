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
    size_t mant_len;
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
    n->deg = 0;
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
    n->mant_len = i;
    printf("%c", n->mant_sign);
    for (int k = 0; k < i; k++)
        printf("%ld", n->mantissa[k]);
    printf("%c", n->deg_sign);
    printf("%ld", n->deg);
    return OK;
}
int minus(long a[MAXLEN], size_t na, long b[MAXLEN], size_t nb)
{
    if (nb > na)
        return ERR;
    if (na == nb)
        for (size_t j = 0; j < na; j++)
        {
            if (a[j] > b[j])
                break;
            if (a[j] < b[j])
                return ERR;
        }
    
    long i = nb - 1, el;
    for (long j = na - 1; j >= 0; j--)
    {
        if (i < 0)
            el = 0;
        else
            el = b[i];
        if (a[j] - el < 0)
        {
            if (a[j - 1] > 0)
            {
                a[j - 1]--;
                a[j] = 10 + a[j] - el;
            }
            else
            {
                long k = 1;
                while (a[j - k] < 0)
                {
                    a[j - k] += 3;
                    k++;
                }
                a[j - k]--;
                a[j] = 10 + a[j] - el;
            }
        }
        else
            a[j] -= el;
        i--;
    }

    return OK;
}
int divide(number *a, number *b, number *r)
{
    //size_t k = 0;
    r->deg = 0;
    r->deg_sign = '+';
    while (a->mant_len < b->mant_len)
    {
        a->mantissa[a->mant_len - 1] = 0;
        a->mant_len++;
        r->deg++;
    }
    if (r->deg > 0)
        r->deg_sign = '-';
    if (a->mant_sign != b->mant_sign)
        r->mant_sign = '-';
    else
        r->mant_sign = '+';
    for (size_t i = b->mant_len; i < a->mant_len; i++)
    {
        while(minus(a->mantissa, a->mant_len, b->mantissa, b->mant_len) == OK)
        {
            long j = 0;
            while((a->mantissa)[j] == )
        }
    }
    
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
//    long a[100], b[100], na, nb;
//    char y;
//    scanf("%ld\n", &na);
//    for (int i = 0; i < na; i++)
//    {
//        scanf("%c", &y);
//        a[i] = (long)y - '0';
//    }
//    scanf("%ld\n", &nb);
//    for (int i = 0; i < nb; i++)
//    {
//        scanf("%c", &y);
//        b[i] = (long)y - '0';
//    }
//    for (size_t l = 0; l < na; l++)
//        printf("%ld ", a[l]);
//    for (size_t l = 0; l < nb; l++)
//        printf("%ld ", b[l]);
//    minus(a, na, b, nb);
