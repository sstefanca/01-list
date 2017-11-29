#ifndef __LIST__H__
#define __LIST__H__ 1

#include <pthread.h>

typedef void (*Tprint) (int);

typedef struct node
{
    Tprint print;
    int val;
    struct node *next;
    //acest lock este redundant
    // daca ar fi posibila promovarea de la read la write,
    // as putea folosi acest lock pentru a permite accesul read
    // la lista in timp ce fac operatii de write
    pthread_rwlock_t lock;
} Tnode;

typedef struct list
{
    Tnode *first, *last;
    pthread_rwlock_t lock;
} Tlist;

Tlist *init_list();
void free_list(Tlist** l);

// Adauga un element la lista
int add_node(Tlist *l, Tprint print, int val);

// Sterge elementul din lista
int delete_node(Tlist *l, int val);

// Afiseaza lista
void print_list(Tlist *l);

// Sorteaza lista in ordine crescatoare
void sort_list(Tlist *l);

// Goleste lista
void flush_list(Tlist *l);

#endif
