#ifndef LIST_H
#define LIST_H

#define LISTINITSIZE 5
#define RESIZE_INCREMENT 3
#define newlist(type) newlist_f(sizeof(type))
#define getp(list,i) *((void **)get(list,i))
#define getlastp(list) *((void **)getlast(list))

typedef struct
{
	int asize; // allocated size
	int lsize; // logical   size
	int esize; // element   size
	void *elements;
} list;

list *newlist_f(int esize);
void add(list *l,void *el);
void addi(list *l,int c);
void addp(list *l,void *ptr);
void *add_dummy(list *l);
void *get(list *l,int index);
void *getlast(list *l);
void *pop(list *l);
void dellist(list *l, void (*freef)(void *));
void map(list *l, void (*mapf)(void *));
void set(list *l,void *el, int i);
list *copystr(list *src);

#endif
