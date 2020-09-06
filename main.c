#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define OK 0
#define ERR 1
#define ERR_LEN -1
#define ERR_NUMBER 3
#define ZERO_DIVISION 2
#define EMPTY -3
#define MAXLEN 30
typedef char string_t[MAXLEN];
typedef struct
{
    char mant_sign;
    long mantissa[2 * MAXLEN];
    size_t mant_len;
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
    short is_dot = 0, is_e = 0, not_zero = 0;
    char sign = '+';
    size_t i = 0, p = 0;
    n->mant_sign = '+';
    n->deg = 0;
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
                sign = *s;
            else
                return ERR;
        }
        else if (isdigit(*s))
        {
            if (is_e)
            {
                n->deg = strtoll(s, NULL, 10);
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
    if (sign == '-')
    {
        n->deg += p;
        n->deg *= -1;
    }
    else
        n->deg -= p;
    n->mant_len = i;
    return OK;
}
int minus(long a[], size_t na, long b[], size_t nb)
{
    long i = nb - 1, el;
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
void divide(number *a, number *b, number *r)
{
    short not_zero = 0;
    long *ptr = &(a->mantissa)[0];
    long count = 0;
    size_t real = a->mant_len;
    r->mant_len = 0;
    r->deg = a->deg - b->deg;
    while (a->mant_len < b->mant_len)
    {
        a->mant_len++;
        a->mantissa[a->mant_len - 1] = 0;
        r->deg--;
    }
    if (a->mant_sign != b->mant_sign)
        r->mant_sign = '-';
    else
        r->mant_sign = '+';
    for (size_t i = b->mant_len; i <= a->mant_len; i++)
    {
        count = 0;
        while(minus(ptr, i, b->mantissa, b->mant_len) == OK)
        {
            count++;
            while(*ptr == 0 && i > 0)
            {
                ptr++;
                a->mant_len--;
                i--;
            }
        }
        if (count || not_zero)
        {
            r->mantissa[(r->mant_len)++] = count;
            not_zero = 1;
        }
        if (i == a->mant_len && i != 0)
        {
            if (r->mant_len < MAXLEN && real < 2 * MAXLEN)
            {
                r->deg--;
                ptr[i] = 0;
                a->mant_len++;
                real++;
            }
            else
                break;
        }
    }
}
void print_num(number *a)
{
    a->deg += a->mant_len;
    if (a->deg > 99999)
    {
        printf("NaN");
        return;
    }
    if (a->deg < -99999)
    {
        printf("0");
        return;
    }
    if (a->mant_sign == '-')
        printf("-");
    printf("0.");
    for (int k = 0; k < a->mant_len; k++)
        printf("%ld", a->mantissa[k]);
    printf("E");
    printf("%ld", a->deg);
}
int main(int argc, char **argv)
{
    string_t s;
    number a, b, r;
    if (read_str(&stdin, s, MAXLEN + 1) < 1)
    {
        printf("String is too large");
        return ERR_LEN;
    }
    if (str_into_num(s, &a) != OK)
    {
        printf("Incorrect format of number");
        return ERR_NUMBER;
    }
    if (read_str(&stdin, s, MAXLEN + 1) < 1)
    {
        printf("String is too large");
        return ERR_LEN;
    }
    if (str_into_num(s, &b) != OK)
    {
        printf("Incorrect format of number");
        return ERR_NUMBER;
    }
    size_t i = 0;
    while (i < b.mant_len)
        if (b.mantissa[i] == 0)
            i++;
        else
            break;
    if (i == b.mant_len || b.deg < -99999)
    {
        printf("Zero division");
        return ZERO_DIVISION;
    }
    divide(&a, &b, &r);
    print_num(&r);
    return OK;
}
