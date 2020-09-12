#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NOTDEBUG 1
#define OK 0
#define ERR 1
#define ERR_LEN -1
#define ERR_NUMBER 3
#define ZERO_DIVISION 2
#define EMPTY -3
#define MAXLEN 30
#define MAXDEG 99999
typedef char string_t[MAXLEN];
// структура имитирует представление действительного числа
typedef struct
{
    char mant_sign;
    short mantissa[2 * MAXLEN + 1];
    size_t mant_len;
    long deg;
} number;

/// Функция читает строку. В случае успеха возвращает длину строки, иначе возвращает отрицательный код.
/// @param f поток, из которого читаются данные
/// @param s строка, в укоторой будут помещены данные
/// @param max максимальная длина строки (должен учитываться нулевой символ)
long read_str(FILE **f, string_t s, size_t max)
{
    size_t i = 0;
    int c = fgetc(*f);
    while (c == ' ')
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

///  Преобразует строку, в которой записано действительное число (в различных форматах), в структуру number. В случае успеха возвращает 0, иначе ненулевое значение.
/// @param s строка с исходным числом
/// @param n указатель на структуру number
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
                char *end = NULL;
                n->deg = strtoll(s, &end, 10);
                if ((*end) != '\0')
                    return ERR;
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
        {
            if (!is_dot)
                is_dot = 1;
            else
                return ERR;
        }
        else if ((*s) == 'E' || (*s) == 'e')
            is_e = 1;
        else
            return ERR;
        s++;
    }
    if (n->deg > MAXDEG || n->deg < -MAXDEG)
        return ERR;
    if (sign == '-')
    {
        n->deg += p;
        n->deg *= -1;
    }
    else
        n->deg -= p;
    n->mant_len = i;
    if (n->mant_len > MAXLEN)
        return ERR;
    return OK;
}

///  Имитирует операцию вычитания a - b, где a и b представлены массивами (целочисленными). Результат будет размещён в массиве a. В случае успеха функция возвращает 0, иначе ненулевое значение.
/// @param a массив, в котором размещено число, из которого будет произведено вычитание
/// @param na длина массива a
/// @param b массив, в котором размещено вычитаемое число
/// @param nb длина массива b
int minus(short a[], size_t na, short b[], size_t nb)
{
    long i = nb - 1, el;
    size_t k = 0;
    // проверка на учёт незначащих нулей
    while (k < na)
        if (a[k] == 0)
            k++;
        else
            break;
    if (k == na)
        return ERR;
    // проверки на возможность совершения операции
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
    // имитация операции вычитания в столбик
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
                    a[j - k] += 9;
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
/// Имитирует операцию деления a на b, где a и b представлены структурами number. Результат будет размещён в структуре r.
/// @param a структура с делимым
/// @param b структура с делителем
/// @param r структура с частным
int divide(number *a, number *b, number *r)
{
    short not_zero = 0;
    short *ptr = &(a->mantissa)[0];
    long count = 0;
    size_t j = 0;
    // проверка на возможность совершения операции
    while (j < b->mant_len)
        if (b->mantissa[j] == 0)
            j++;
        else
            break;
    if (j == b->mant_len)
        return ZERO_DIVISION;
    size_t real = a->mant_len;
    r->mant_len = 0;
    r->deg = a->deg - b->deg;
    // дополнение нулями делимое и уменьшение порядка результата, если количество символов в делимом меньше, чем в делителе
    while (a->mant_len < b->mant_len)
    {
        a->mant_len++;
        a->mantissa[a->mant_len - 1] = 0;
        r->deg--;
    }
    // определение знака результата
    if (a->mant_sign != b->mant_sign)
        r->mant_sign = '-';
    else
        r->mant_sign = '+';
    // имитация деления в столбик
    for (size_t i = b->mant_len; i <= a->mant_len; i++)
    {
        // производим вычитание делителя из части делимого до тех пор, пока возможно, и считаем количество успешных вичитаний
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
        // записываем количество вычитаний в мантиссу результата (не учитываем незначащие нули)
        if (count || not_zero)
        {
            r->mantissa[(r->mant_len)++] = count;
            not_zero = 1;
        }
        // если в делимом больше нет цифр, а остаток от деления есть, то дописываем ноль к делимому и уменьшаем порядок результата
        // делаем это до тех пор, пока в мантиссе результата есть место или до тех пор, пока размер массива с делимым позволяет дописывать нули
        if (i == a->mant_len && i != 0)
        {
            if (r->mant_len < MAXLEN + 1 && real < 2 * MAXLEN + 1)
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
    return OK;
}
///  имитация операции a + 1, где a - масиив, в котором записано число
/// @param a массив с числом
/// @param n  его длина
/// @param is_full отслеживание переполнения
void inc_num(short a[], size_t *n, int *is_full)
{
    *is_full = 0;
    a[*n - 1]++;
    long i = *n - 1;
    while (a[i] == 10)
    {
        a[i] = 0;
        (*n)--;
        a[i - 1]++;
        i--;
        if (i == 0 && a[i] == 10)
        {
            a[i] = 1;
            *is_full = 1;
        }
    }
}
/// Выводит действительное число на экран в нормализованном виде
/// @param a структура number
void print_num(number *a)
{
    a->deg += a->mant_len;
    if (a->deg > MAXDEG)
    {
        printf("infinity");
        return;
    }
    if (a->deg < -MAXDEG)
    {
        printf("0");
        return;
    }
    if (a->mant_sign == '-')
        printf("-");
    printf("0.");
    if (a->mant_len == MAXLEN + 1)
    {
        // округление числа
        if (a->mantissa[a->mant_len - 1] >= 5)
        {
            int is_full;
            a->mant_len--;
            inc_num(a->mantissa, &(a->mant_len), &is_full);
            if (is_full)
                a->deg++;
        }
        else
            a->mant_len--;
    }
    while (!a->mantissa[a->mant_len - 1])
        a->mant_len--;
    for (size_t k = 0; k < a->mant_len; k++)
        printf("%d", a->mantissa[k]);
    printf("E");
    printf("%ld", a->deg);
}
void fill_zero(short *a, size_t n)
{
    for (size_t i = 0; i < n; i++)
        a[i] = 0;
}
int main()
{
    string_t s;
    number a, b, r;
    fill_zero(a.mantissa, 2 * MAXLEN + 1);
    fill_zero(b.mantissa, 2 * MAXLEN + 1);
    fill_zero(r.mantissa, 2 * MAXLEN + 1);
    if (NOTDEBUG)
    {
        printf("This program divides integer number on a real\n");
        printf("Length of integer number and manttissa of real number <= 30 (not empty);\n");
        printf("Degree of numbers in range of: -99999 <= deg <= 99999;\n");
        printf("The result will be presented in a following format:\n(+/-)0.<mantissa>E(+/-)<degree>\n");
        printf("Please, follow some rules :\n- use dot (not a comma) for real numbers;\n- don't use spaces between parts of number;\n- after mantissa you may(!) use format: e/E<degree>;\n");
        printf("Input dividend: ");
    }
    if (read_str(&stdin, s, MAXLEN + 10) < 1)
    {
        printf("String is empty or too large\n");
        return ERR_LEN;
    }
    if (str_into_num(s, &a) != OK)
    {
        printf("Incorrect format of number\n");
        return ERR_NUMBER;
    }
    if (NOTDEBUG)
        printf("Input divider: ");
    
    if (read_str(&stdin, s, MAXLEN + 10) < 1)
    {
        printf("String is empty or too large\n");
        return ERR_LEN;
    }
    if (str_into_num(s, &b) != OK)
    {
        printf("Incorrect format of number\n");
        return ERR_NUMBER;
    }
    if (divide(&a, &b, &r) != OK)
    {
        printf("Zero division\n");
        return ZERO_DIVISION;
    }
    if (NOTDEBUG)
        printf("Result: ");
    print_num(&r);
    return OK;
}

