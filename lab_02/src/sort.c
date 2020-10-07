#include "sort.h"

void swap(char *p1, char *p2, size_t size)
{
    if (!size || !p1 || !p1)
        return;
    char q;
    for(size_t i = 0; i < size; i++)
    {
        q = *p1;
        *p1 = *p2;
        *p2 = q;
        p1++;
        p2++;
    }
}
void min_max_sort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *))
{
    if (beg == NULL || comparator == NULL)
        return;
    char *pb = (char *)beg;
    char *pe = pb + size * number;
    if (pb == pe)
        return;
    pe -= size;
    char *ptr = NULL, *max_ptr = NULL;
    while (pe > pb)
    {
        max_ptr = ptr = pe;
        while (ptr >= pb)
        {
            if (comparator(ptr, max_ptr) > 0)
                max_ptr = ptr;
            ptr -= size;
        }
        if (max_ptr != pe)
            swap(max_ptr, pe, size);
        pe -= size;
    }
    return;
}
void quicksort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *))
{
    number--;
    char *a = (char *) beg;
    long i = 0, j = number;
    long mid = number / 2;
    while (i <= j)
    {
        while (comparator(a + mid * size, a + i * size) > 0)
            i++;
        while (comparator(a + j * size, a + mid * size) > 0)
            j--;
        if (i <= j)
        {
            if (i == mid)
                mid = j;
            else if (j == mid)
                mid = i;
            swap(a + i * size, a + j * size, size);
            i++;
            j--;
        }
    }
    if (j > 0)
        quicksort(a, j + 1, size, comparator);
    if (i < (long) number)
        quicksort(a + i * size, number - i + 1, size, comparator);
}
void heapify (char a[], size_t size, size_t l, size_t r,  int (* comparator) (const void *, const void *))
{
    size_t i = l, j = 2 * l + 1;
    if (j < r && comparator(a + (j + 1) * size, a + j * size) > 0)
        j++;
    while (j <= r && comparator(a + j * size, a + i * size) > 0)
    {
        swap(a + i * size, a + j * size, size);
        i = j;
        j = 2 * j + 1;
        if (j < r && comparator(a + (j + 1) * size, a + j * size) > 0)
            j++;
    }
}

void heap_sort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *))
{
    char *a = (char *) beg;
    size_t l = number / 2, r = number - 1;
    while (l > 0)
    {
        l--;
        heapify(a, size, l, r, comparator);
    }
    while (r > 0) {
        swap(a, a + r * size, size);
        r--;
        heapify(a, size, l, r, comparator);
    }
}

//void heapify(char a[], size_t size, size_t n, size_t i,  int (* comparator) (const void *, const void *))
//{
//    size_t max = i, l = 2 * i + 1, r = 2 * i + 2;
//    if (l < n && comparator((a + l * size), (a + max * size)) > 0)
//        max = l;
//    if (r < n && comparator((a + r * size), (a + max * size)) > 0)
//        max = r;
//    if (i != max)
//    {
//        swap(a + max * size, a + i * size, size);
//        heapify(a, size, n, max, comparator);
//    }
//}
//void heap_sort(void * beg, size_t number, size_t size, int (* comparator) (const void *, const void *))
//{
//    char *a = (char *) beg;
//    for (long i = number / 2 - 1; i >= 0; i--)
//        heapify(a, size, number, i, comparator);
//    for (long i = number - 1; i >= 0; i--)
//    {
//        swap(a, a + i * size, size);
//        heapify(a, size, i, 0, comparator);
//    }
//}

//void heapify(char a[], size_t size, size_t bottom, size_t root,  int (* comparator) (const void *, const void *))
//{
//    size_t maxChild;
//    int done = 0;
//    while ((root * 2 + 1 <= bottom) && (!done))
//    {
//        if (root * 2 + 1 == bottom)
//            maxChild = root * 2 + 1;
//        else if (comparator(a + (root * 2 + 1) * size, a + (root * 2 + 2) * size) > 0)
//            maxChild = root * 2  + 1;
//        else
//            maxChild = root * 2 + 2;
//        if (comparator(a + maxChild * size, a + root * size) > 0)
//        {
//            swap(a + root * size, a + maxChild * size, size);
//            root = maxChild;
//        }
//        else
//            done = 1;
//    }
//}
//void heap_sort(void * beg, size_t number,size_t size, int (* comparator) (const void *, const void *))
//{
//    char *a = (char *) beg;
//    for (long i = (number / 2) - 1; i >= 0; i--)
//        heapify(a, size, number - 1, i, comparator);
//    for (long i = number - 1; i >= 1; i--)
//    {
//        swap(a, a + i * size, size);
//        heapify(a, size, i - 1, 0, comparator);
//    }
//}

