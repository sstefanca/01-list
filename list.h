#ifndef __LIST__H__
#define __LIST__H__ 1

#include <pthread.h>

typedef void (*Tprint) (void *);

typedef struct node
{
    Tprint print;
    int val;
    struct node *next;
    //TODO: node sync lock
} Tnode;

typedef struct list
{
    Tnode *first, *last;
    //TODO: list sync lock
} Tlist;

Tlist *init_list();
void free_list(Tlist** l);

// Adauga un element la lista
int add_node(Tlist *l, Tprint print, int val);

// Sterge elementul din lista
int delete_node(Tlist *l, int val);

// Afiseaza lista
void print_list();

// Sorteaza lista in ordine crescatoare
void sort_list();

// Goleste lista
void flush_list();

#endif
