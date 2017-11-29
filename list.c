#include "list.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

static Tnode *init_node()
{
    Tnode *ret = calloc(1, sizeof(*ret));
    int retval;
    retval = pthread_rwlock_init(&(ret->lock), NULL);
    if(retval!=0)
    {
	free(ret);
	ret = 0;
    }
    return ret;
}

static void free_node(Tnode **n)
{
    int retval;
    retval = pthread_rwlock_destroy(&((*n)->lock));
    if(retval!=0)
    {
	//nu ar trebui sa intre aici
	fprintf(stderr, "ERROR: Tried to destroy locked lock\n");
	exit(1);
    }
    free(*n);
    *n=NULL;
}

Tlist *init_list()
{
    int retval;
    Tlist *ret = calloc(1, sizeof(*ret));
    retval = pthread_rwlock_init(&(ret->lock), NULL);
    if(retval!=0)
    {
	free(ret);
	ret = 0;
    }
    return ret;
}

void free_list(Tlist **l)
{
    int retval;

    flush_list(*l);
    
    retval = pthread_rwlock_destroy(&((*l)->lock));
    if(retval!=0)
    {
	//nu ar trebui sa intre aici
	fprintf(stderr, "ERROR: Tried to destroy locked lock\n");
	exit(1);
    }
    
    free(*l);
    *l=NULL;
}

int add_node(Tlist *l, Tprint print, int val)
{
    Tnode *node;

    if(l == NULL || print == NULL)
	return 1;
    
    node = init_node();
    if(node == NULL)
	return 2;
    node->val=val;
    node->print=print;

    pthread_rwlock_wrlock(&node->lock);
    pthread_rwlock_wrlock(&l->lock);
    if(l->first == NULL)
    {
	l->first = node;
	l->last = node;
    }
    else
    {
	pthread_rwlock_wrlock(&l->last->lock);
	pthread_rwlock_t *lock = &l->last->lock;
	l->last->next = node;
	l->last = node;
	pthread_rwlock_unlock(lock);
    }
    pthread_rwlock_unlock(&node->lock);
    pthread_rwlock_unlock(&l->lock);
    
    return 0;
}

int delete_node(Tlist *l, int val)
{
    Tnode *it, *pit = NULL, *aux;
    int ret = 0;

    if(l == NULL)
	return ret;

    pthread_rwlock_wrlock(&l->lock);
    if(l->first == NULL)
    {
	pthread_rwlock_unlock(&l->lock);
	return ret;
    }
    
    pthread_rwlock_wrlock(&l->first->lock);
    if(l->first->val == val)
    {
	aux = l->first;
	//deja avem wrlock
	//pthread_rwlock_wrlock(&l->lock);
	//pthread_rwlock_wrlock(&aux->lock);
	l->first = aux->next;
	if(l->first == NULL)
	    l->last = NULL;
	ret = aux -> val;

	/*
	  // sunt necesare aceste doua linii? daca e ceva, asigura ca nu mai sunt rdlockuri
	pthread_rwlock_unlock(&aux->lock);
	pthread_rwlock_wrlock(&aux->lock);
	*/

	pthread_rwlock_unlock(&l->lock);
	pthread_rwlock_unlock(&aux->lock);
	free_node(&aux);
	
	return ret;
    }

    /*
//Am decis ca aceasta secventa este redundanta
    if(l->first->next == NULL)
    {
	pthread_rwlock_unlock(&l->first->lock);
	pthread_rwlock_unlock(&l->lock);
	return ret;
    }
    */
    
    it = l->first->next;
    pit = l->first;
    while(it != NULL)
    {
	pthread_rwlock_wrlock(&it->lock);
	if(it->val == val)
	{
	    pit->next = it->next;
	    if(l->last == it)
		l->last = pit;
	    pthread_rwlock_unlock(&pit->lock);

	    ret = it->val;
	    pthread_rwlock_unlock(&it->lock);
	    free_node(&it);
	    pthread_rwlock_unlock(&l->lock);
	    return ret;
	}
	pthread_rwlock_unlock(&pit->lock);
	pit = it;
	it = it->next;
    }

    pthread_rwlock_unlock(&pit->lock);
    pthread_rwlock_unlock(&l->lock);
    return ret;
}

void print_list(Tlist *l)
{
    Tnode *it, *pit;

    printf("(");
    pthread_rwlock_rdlock(&l->lock);

    if(l->first != NULL)
    {
	pthread_rwlock_rdlock(&l->first->lock);
	pit = l->first;
	pit->print(pit->val);
	printf(", ");
    }
    it = pit->next;
    while(it != NULL)
    {
	pthread_rwlock_rdlock(&it->lock);
	it->print(it->val);
	pthread_rwlock_unlock(&pit->lock);
	pit = it;
	it = it->next;
	printf(", ");
    }
    printf(")\n");
    pthread_rwlock_unlock(&pit->lock);
    pthread_rwlock_unlock(&l->lock);
}

void sort_list(Tlist *l)
{
    Tnode *it, *ait;

    //TODO: write lock list and nodes
    pthread_rwlock_wrlock(&l->lock);
    for(it=l->first; it!=NULL; it=it->next)
	pthread_rwlock_wrlock(&it->lock);
    
    for( it = l->first; it!=NULL; it=it->next)
	for( ait = it->next; ait!=NULL; ait=ait->next)
	{
	    if(ait->val < it->val)
	    {
		Tprint auxp;
		int auxi;

		auxp = ait->print;
		ait->print = it->print;
		it->print = auxp;

		auxi = ait->val;
		ait->val = it->val;
		it->val = auxi;
	    }
	}

    //TODO: release locks
    for(it=l->first; it!=NULL; it=it->next)
	pthread_rwlock_unlock(&it->lock);
    pthread_rwlock_unlock(&l->lock);
}

void flush_list(Tlist *l)
{
    Tnode *it, *aux;

    pthread_rwlock_wrlock(&l->lock);
    it = l->first;
    l->first = NULL;
    l->last = NULL;
    pthread_rwlock_unlock(&l->lock);
    
    while(it!=NULL)
    {
	aux = it;
	pthread_rwlock_wrlock(&aux->lock);
	it = it->next;
	pthread_rwlock_unlock(&aux->lock);
	free_node(&aux);
    }
}
