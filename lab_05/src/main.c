#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "element.h"
#include "queue_on_array.h"
#include "queue_on_list.h"

#define OK                   0
#define ERR                  1
#define ERR_LEN              -1
#define ERR_READ             2
#define MAXLEN               10000000
#define YES                  1
#define NO                   0

#define COUNT 1000
#define T1    5
#define T2    4
#define T3    4

#define EXIT                                 0
#define CHANGE_TIME_INTERVAL_BETWEEN_FIRST   1
#define CHANGE_TIME_OF_PROCESSING_FIRST      2
#define CHANGE_TIME_OF_PROCESSING_SECOND     3
#define SIMULATE_WITH_QUEUE_ON_ARRAY         4
#define SIMULATE_WITH_QUEUE_ON_LIST          5
#define STATISTICS                           6

unsigned time_int = T1, time_proc_first = T2, time_proc_second = T3;

void print_menu()
{
    printf(
        "EXIT                                 0\n"
        "CHANGE TIME INTERVAL BETWEEN FIRST   1\n"
        "CHANGE TIME OF PROCESSING FIRST      2\n"
        "CHANGE TIME OF PROCESSING SECOND     3\n"
        "SIMULATE WITH QUEUE ON ARRAY         4\n"
        "SIMULATE WITH QUEUE ON LIST          5\n"
        "STATISTICS                           6\n");
}
void init_elem(elem_t *elem, int seed_of_proc, double beg_time, type_t type)
{
    if (elem)
    {
        elem->procesing_time = ( rand() % (seed_of_proc * 1000 + 1) ) / 1000.0;
        elem->begin_time = beg_time;
        elem->type = type;
    }
}
int fill_time_with_second(void *queue, double free_time, double time_from_beg,  void * (*get_front)(void *), void * (*pop)(void *), int (* insert)(void *, void *), int adr)
{
    double done = 0;
    elem_t *tmp;
    int count = 0;
    while ((tmp = get_front(queue)) != NULL && tmp->type == SECOND && tmp->procesing_time <= free_time - done)
    {
        tmp = pop(queue);
        if (adr)
            print_queue_on_list(stdout, queue, print_elem);
        done += tmp->procesing_time;
        count++;
        init_elem(tmp, time_proc_second, time_from_beg + done, SECOND);
        if (insert(queue, tmp) != OK)
            return -1;
        if (adr)
            print_queue_on_list(stdout, queue, print_elem);
    }
    return count;
}
//void * get_front_in_queue_on_array(void *q);
//
//int insert_to_queue_on_array(void *q, void *value);
//
//size_t get_length_of_array(void *q);
int process_queue(FILE *f, void *queue, int (*push)(void *, void *), void * (*pop)(void *), void * (*get_front)(void *), int (* insert)(void *, void *), size_t (* get_length)(void *), int print, size_t *len, int adr)
{
#define EPS 1e-9
    unsigned out_first = 0, in_first = 0, count_second;
    double time_interval = 0, time_from_beg = 0, next_time = 0, av_time = 0, time_of_free_machine = 0;
    size_t max_length = 0;
    
    elem_t *elem = reader_elem(f), *tmp = NULL;
    init_elem(elem, time_proc_second, time_from_beg, SECOND);
    if (push(queue, elem) != OK)
        return ERR;
    if (adr)
        print_queue_on_list(stdout, queue, print_elem);
    next_time = ( rand() % (time_int * 1000 + 1) ) / 1000.0;
    count_second = fill_time_with_second(queue, next_time, time_from_beg, get_front, pop, insert, adr);
    if (count_second == -1)
        return ERR;
    time_from_beg = next_time;
    elem = reader_elem(f);
    if (elem)
    {
        in_first = 1;
        init_elem(elem, time_proc_first, time_from_beg, FIRST);
        if (push(queue, elem) != OK)
            return ERR;
        if (adr)
            print_queue_on_list(stdout, queue, print_elem);
        next_time = ( rand() % (time_int * 1000 + 1) ) / 1000.0;
        time_interval = next_time;
    }
    size_t av_len = 0;
    while (out_first < COUNT)
    {
        double done = 0;
        while ((tmp = get_front(queue)) != NULL && tmp->procesing_time <= time_interval)
        {
            if (get_length(queue) > max_length)
                max_length = get_length(queue);
            tmp = pop(queue);
            if (adr)
                print_queue_on_list(stdout, queue, print_elem);
            time_interval -= tmp->procesing_time;
            av_time += time_from_beg + done - tmp->begin_time;
            done += tmp->procesing_time;
            if (tmp->type == SECOND)
            {
                count_second++;
                init_elem(tmp, time_proc_second, time_from_beg + done, SECOND);
                if (insert(queue, tmp) != OK)
                    return ERR;
                if (adr)
                    print_queue_on_list(stdout, queue, print_elem);
                if (get_length(queue) > max_length)
                    max_length = get_length(queue);
                av_len += get_length(queue);
            }
            else
            {
                out_first++;
                if (out_first % 100 == 0 && out_first != 0)
                {
                    if (print)
                    {
                        printf("first out: %u\n", out_first);
                        printf("first in %u, first out: %u\n", in_first, out_first);
                        printf("second count: %u\n", count_second);
                        printf("average time in queue: %lf\n", av_time / (out_first + count_second));
                        printf("current length of queue: %lf\n", 1.0 * get_length(queue));
                        printf("average length of queue: %lf\n", 1.0 * av_len / (in_first + count_second));
                        printf("\n");
                    }
                }
                free_elem(tmp);
            }
        }
        time_from_beg += next_time;
        elem = reader_elem(f);
        if (elem)
        {
            in_first++;
            if (tmp == NULL && time_interval > 0)
            {
                time_of_free_machine += time_interval;
                time_interval = 0;
            }
            init_elem(elem, time_proc_first, time_from_beg, FIRST);
            if (push(queue, elem) != OK)
                return ERR;
            if (adr)
                print_queue_on_list(stdout, queue, print_elem);
            if (get_length(queue) > max_length)
                max_length = get_length(queue);
            av_len += get_length(queue);
            next_time = ( rand() % (time_int * 1000 + 1) ) / 1000.0;
            time_interval += next_time;
        }
        else
            break;
    }
    if (print)
    {
        printf("time from begining: %lf\n", time_from_beg);
        printf("time of free machine: %lf\n", time_of_free_machine);
        printf("first in %u, first out: %u\n", in_first, out_first);
        printf("second count: %u\n", count_second);
        printf("average time in queue: %lf\n", av_time / (out_first + count_second));
        printf("counted time: %u * %.2lf + %u * %.2lf = %.2lf + %.2lf = %.2lf\n",
               out_first,
               time_proc_first / 2.0,
               count_second,
               time_proc_second / 2.0,
               out_first * time_proc_first / 2.0,
               count_second * time_proc_second / 2.0,
               out_first * time_proc_first / 2.0 + count_second * time_proc_second / 2.0);
        if (out_first * time_proc_first / 2.0 + count_second * time_proc_second / 2.0 > time_from_beg)
            printf("percent: 100 - ( %.2lf / %.2lf) * 100 = %.4lf\n",
                   time_from_beg,
                   out_first * time_proc_first / 2.0 + count_second * time_proc_second / 2.0,
                   100 - time_from_beg / (out_first * time_proc_first / 2.0 + count_second * time_proc_second / 2.0) * 100.0);
        else
            printf("percent: 100 - (%.2lf / %.2lf) * 100 = %.4lf\n",
            out_first * time_proc_first / 2.0 + count_second * time_proc_second / 2.0,
            time_from_beg,
            100 - (out_first * time_proc_first / 2.0 + count_second * time_proc_second / 2.0) / time_from_beg * 100.0);
    }
    *len += max_length;
    return OK;
}
void print_int(FILE *f, const int *elem)
{
    fprintf(f, "%d", *elem);
}
int statistics(char *argv)
{
    struct timeval tv_start, tv_stop;
    int64_t time = 0;
    size_t size = 0;
    printf("x--------------- x---------------x---------------x\n");
    printf("|     queue      |     time      |     memory    |\n");
    printf("x----------------x---------------x---------------x\n");
    printf("| on linked list |");
    FILE *in_stream;
    if ((in_stream = fopen(argv, "r")) == NULL)
    {
        printf("error while opening file\n");
        return ERR_READ;
    }
    queue_on_list_t queue;
    init_queue_on_list(&queue);
    int rv;
    if ((rv = process_queue(in_stream, &queue, push_to_queue_on_list, pop_from_queue_on_list, get_front_in_queue_on_list, insert_to_queue_on_list, get_length_in_list, 0, &size, 0)) != OK)
    {
        printf("%15s|", "-");
        printf("%15s|", "-");
        printf(": memory error");
        printf("\n");
        delete_queue_on_list(&queue, free_elem);
    }
    else
    {
        delete_queue_on_list(&queue, free_elem);
        size = 0;
        for (size_t j = 0; j < 10; j++)
        {
            rewind(in_stream);
            init_queue_on_list(&queue);
            gettimeofday(&tv_start, NULL);
            process_queue(in_stream, &queue, push_to_queue_on_list, pop_from_queue_on_list, get_front_in_queue_on_list, insert_to_queue_on_list, get_length_in_list, 0, &size, 0);
            gettimeofday(&tv_stop, NULL);
            delete_queue_on_list(&queue, free_elem);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        printf("%15.2lf|", time / 10.0);
        printf("%15.2lf|\n", size / 10.0 * sizeof(node_t));
    }
    fclose(in_stream);
    printf("x----------------x---------------x---------------x\n");
    printf("|   on vector    |");
    size = 0;
    time = 0;
    if ((in_stream = fopen(argv, "r")) == NULL)
    {
        printf("error while opening file\n");
        return ERR_READ;
    }
    queue_on_array_t q;
    init_queue_on_array(&q, MAXLEN);
    if ((rv = process_queue(in_stream, &q, push_to_queue_on_array, pop_from_queue_on_array, get_front_in_queue_on_array, insert_to_queue_on_array, get_length_of_array, 0, &size, 0)) != OK)
    {
        printf("%15s|", "-");
        printf("%15s|", "-");
        printf(": queue overflow");
        printf("\n");
        delete_queue_on_array(&q, free_elem);
    }
    else
    {
        delete_queue_on_array(&q, free_elem);
        size = 0;
        for (size_t j = 0; j < 10; j++)
        {
            rewind(in_stream);
            init_queue_on_array(&q, 1000000);
            gettimeofday(&tv_start, NULL);
            process_queue(in_stream, &q, push_to_queue_on_array, pop_from_queue_on_array, get_front_in_queue_on_array, insert_to_queue_on_array, get_length_of_array, 0, &size, 0);
            gettimeofday(&tv_stop, NULL);
            delete_queue_on_array(&q, free_elem);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        printf("%15.2lf|", time / 10.0);
        printf("%15.2lf|\n", size / 10.0 * sizeof(elem_t *));
    }
    fclose(in_stream);
    printf("x----------------x---------------x---------------x\n");
    return OK;
}
int main(int argc, char **argv)
{
    FILE *in_stream;
    if (argc < 2 || (in_stream = fopen(argv[1], "r")) == NULL)
    {
        printf("error while opening file\n");
        return ERR_READ;
    }
    printf("Program shows working of queues based on dynamic array and list.\n");
    printf("This program reads applications from file (name should be given in command line args) and simulates the processing of applications that are queued as they are received (when 1000 applications of first type were processed, program ends work).\n");
    int choise;
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
            case CHANGE_TIME_INTERVAL_BETWEEN_FIRST:
            {
                if (scanf("%u", &time_int) != 1)
                {
                    printf("errors while reading\n");
                    return ERR_READ;
                }
                break;
            }
            case CHANGE_TIME_OF_PROCESSING_FIRST:
                if (scanf("%u", &time_proc_first) != 1)
                {
                    printf("errors while reading\n");
                    return ERR_READ;
                }
                break;
            case CHANGE_TIME_OF_PROCESSING_SECOND:
                if (scanf("%u", &time_proc_second) != 1)
                {
                    printf("errors while reading\n");
                    return ERR_READ;
                }
                break;
            case SIMULATE_WITH_QUEUE_ON_ARRAY:
            {
                if ((in_stream = fopen(argv[1], "r")) == NULL)
                {
                    printf("error while opening file\n");
                    return ERR_READ;
                }
                queue_on_array_t queue;
                init_queue_on_array(&queue, MAXLEN);
                size_t s = 0;
                process_queue(in_stream, &queue, push_to_queue_on_array, pop_from_queue_on_array, get_front_in_queue_on_array, insert_to_queue_on_array, get_length_of_array, 1, &s, 0);
                delete_queue_on_array(&queue, free_elem);
                fclose(in_stream);
                break;
            }
            case SIMULATE_WITH_QUEUE_ON_LIST:
            {
                if ((in_stream = fopen(argv[1], "r")) == NULL)
                {
                    printf("error while opening file\n");
                    return ERR_READ;
                }
                queue_on_list_t queue;
                init_queue_on_list(&queue);
                size_t s = 0;
                int adr;
                printf("print adrs? 0 - no, 1 - yes\n");
                if (scanf("%d", &adr) != 1 || adr > 1 || adr < 0)
                {
                    printf("errors while reading\n");
                    return ERR_READ;
                }
                process_queue(in_stream, &queue, push_to_queue_on_list, pop_from_queue_on_list, get_front_in_queue_on_list, insert_to_queue_on_list, get_length_in_list, 1, &s, adr);
                delete_queue_on_list(&queue, free_elem);
                fclose(in_stream);
                break;
            }
            case STATISTICS:
                statistics(argv[1]);
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
    fclose(in_stream);
    return OK;
}
//    queue_on_array_t queue;
//    init_queue_on_array(&queue, 10);
//    int a = 9;
//    insert_to_queue_on_array(&queue, &a);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int b = 8;
//    insert_to_queue_on_array(&queue, &b);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    pop_from_queue_on_array(&queue);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int c = 7;
//    insert_to_queue_on_array(&queue, &c);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int d = 6;
//    insert_to_queue_on_array(&queue, &d);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    pop_from_queue_on_array(&queue);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int e = 5;
//    insert_to_queue_on_array(&queue, &e);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int g = 4;
//    insert_to_queue_on_array(&queue, &g);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int h = 3;
//    insert_to_queue_on_array(&queue, &h);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    int j = 2;
//    insert_to_queue_on_array(&queue, &j);
//    print_queue_on_array(stdout, &queue, print_int);
//    printf("head: ");
//    print_int(stdout, queue.data[queue.head]);
//    printf("\n");
//    fclose(in_stream);
//    return 0;
