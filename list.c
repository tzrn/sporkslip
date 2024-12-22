#include <string.h>
#include <stdlib.h>
#include "list.h"

list *newlist_f(int esize)
{
	list *l=malloc(sizeof(list));
	l->asize=LISTINITSIZE;
	l->esize=esize;
	l->lsize=0;
	l->elements=malloc(esize*LISTINITSIZE);
	return l;
}

void adjust_size(list *l)
{
	if(l->lsize>=l->asize)
	{
		l->asize+=RESIZE_INCREMENT;
		l->elements=realloc(l->elements,l->asize*l->esize);
	}
}

list *copystr(list *src)
{
	list *copy=malloc(sizeof(list));
	copy->lsize=src->lsize; //or just *copy=*src?
	copy->asize=src->asize;
	copy->esize=src->esize;
	copy->elements=malloc(copy->esize*copy->asize);
	strcpy(copy->elements,src->elements);
	return copy;
}

#define element ((char *)l->elements)+l->esize
void *get(list *l,int index) { return element * index;        }
void *getlast(list *l)       { return element * (l->lsize-1); }
void *pop(list *l)           { return element * --l->lsize;   }

void set(list *l,void *el, int i)
{
	//printf("cpy to %p from %p, %d bytes\n",l->elements+i*l->esize,el,l->esize);
	memcpy(element * l->lsize,el,l->esize);
}

void add(list *l,void *el)
{
	adjust_size(l);
	memcpy(element * l->lsize,el,l->esize);
	l->lsize++;
}
void *add_dummy(list *l)
{
	adjust_size(l);
	l->lsize++;
	return getlast(l);
}

void addi(list *l,int c)     { add(l,&c);   }
void addp(list *l,void *ptr) { add(l,&ptr); }

void dellist(list *l, void (*freef)(void *))
{
	if(freef!=NULL)
	{
		for(int i=0;i<l->lsize;i++)
		{
			freef(getp(l,i));
		}
	}
	free(l->elements);
	free(l);
}

void map(list *l, void (*mapf)(void *))
{
	for(int i=0;i<l->lsize;i++)
	{
		mapf(getp(l,i));
	}
}
