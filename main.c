#include <stdio.h>
#include <pthread.h>

#include "list.h"

#define THREAD_NUM 3

Tlist *l;
pthread_barrier_t bar;
pthread_t th1,th2,th3;

void printElem(int i)
{
    printf("%i", i);
}

// result 4

void *t1(void *v)
{
    pthread_barrier_wait(&bar);
    add_node(l, printElem, 2);
    add_node(l, printElem, 4);
    add_node(l, printElem, 10);
    delete_node(l, 2);
    sort_list(l);
    delete_node(l, 10);
    delete_node(l, 5);

    return NULL;
}

// result 1, 8

void *t2(void *v)
{
    pthread_barrier_wait(&bar);
    add_node(l, printElem, 11);
    add_node(l, printElem, 1);
    delete_node(l, 11);
    add_node(l, printElem, 8);
    printf("TH2: ");
    print_list(l);

    return NULL;
}

// result 25, 30

void *t3(void *v)
{
    pthread_barrier_wait(&bar);
    add_node(l, printElem, 30);
    add_node(l, printElem, 25);
    add_node(l, printElem, 100);
    sort_list(l);
    printf("TH3: ");
    print_list(l);
    delete_node(l, 100);

    return NULL;
}

// result 1, 4, 8, 25, 30 (any order)

int main()
{
    l = init_list();
    pthread_barrier_init(&bar, NULL, THREAD_NUM);

    pthread_create(&th1, NULL, t1, NULL);
    pthread_create(&th2, NULL, t2, NULL);
    pthread_create(&th3, NULL, t3, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    printf("MASTER: ");
    print_list(l);
    free_list(&l);
    pthread_barrier_destroy(&bar);
    
    return 0;
}
