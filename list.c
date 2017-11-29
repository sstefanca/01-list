#include "list.h"
#include <stdlib.h>
#include <pthread.h>

static Tnode *init_node()
{
    Tnode *ret = calloc(1, sizeof(*ret));
    //TODO: init lock
    return ret;
}

static void free_node(Tnode **n)
{
    //TODO: destroy lock
    free(*n);
    *n=NULL;
}

Tlist *init_list()
{
    Tlist *ret = calloc(1, sizeof(*ret));
    //TODO: init lock
    return ret;
}

void free_list(Tlist **l)
{
    //TODO: destroy lock
    flush_list(*l);
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

    //TODO: list sync required
    if(l->first == NULL)
    {
	l->first = node;
	l->last = node;
    }
    else
    {
	//TODO: node sync required
	l->last->next = node;
	l->last = node;
	//TODO: release node sync
    }
    //TODO: release list sync
    
    return 0;
}

int delete_node(Tlist *l, int val)
{
    Tnode *it, *pit = NULL, *aux;
    int ret = 0;

    if(l == NULL)
	return 0;

    if(l->first->val == val)
    {
	//TODO: lock list
	//TODO: lock elem
	aux = l->first;
	l->first = aux->next;
	if(l->first == NULL)
	    l->last = NULL;
	ret = aux -> val;
	
	//TODO: release lock list
    }
    it = l->first;
    while(it != NULL)
    {
	if(it->val == val)
	{
	    pit->next = it->next;
	    free_node(&it);
	}
	pit = it;
	it = it->next;
    }

    return ret;
}

void print_list(Tlist *l)
{
    Tnode *it;

    it = l->first;
    while(it != NULL)
    {
	it->print(it->val);
	it = it->next;
    }
}

void sort_list(Tlist *l)
{
    Tnode *it, *ait;

    //TODO: write lock list and nodes
    
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
}

void flush_list(Tlist *l)
{
    Tnode *it, *aux;

    it = l->first;
    l->first = NULL;
    l->last = NULL;

    while(it!=NULL)
    {
	aux = it;
	it = it->next;
	free_node(&aux);
    }
}
