#include "hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//TOMBSTONE IS A BUG, BAD REGROW ADDITION

unsigned int hashstr(char *str)
{
        int value=1234567890; //take some random intial value
        while(*str)
        {
            value ^= *str;
            value *= 12345; //spread around
			str++;
        }
        return value;
}

table_entry *find_entry(table_entry *entries, int asize,char *key)
{
	if(asize<=0)return NULL;
	int index = hashstr(key)%asize;
	table_entry *tombstone = NULL;

	for(;;)
	{
		table_entry *entry=entries+index;

		if(entry->key==NULL)
			if(entry->value==(void*)TOMBSTONE)
				tombstone=entry;
			else
				return tombstone?tombstone:entry;
		else
			if(strcmp(entry->key,key)==0)
				return entry;

		index = (index+1)%asize;
	}
}

void adjust_capacity(hash_table *t, int capacity)
{
	table_entry *entries = malloc(sizeof(table_entry)*capacity);
	for(int i=0;i<capacity;i++)
	{
		entries[i].key=NULL;
		entries[i].value=NULL;
	}

	for(int i=0;i<t->asize;i++)
	{
		table_entry *entry=t->entries+i;
		if(entry->key==NULL)continue;
		
		table_entry *new_home = find_entry(entries,capacity,entry->key);
		new_home->key=entry->key;
		new_home->value=entry->value;
	}
	
	if(t->entries!=NULL)
		free(t->entries);
	t->entries=entries;
	t->asize=capacity;
}

void table_set(hash_table *table, char *key, void *value)
{	
	if(table->asize*TABLE_MAX_LOAD<=table->lsize)
		adjust_capacity(table,table->asize+2);
	
	table_entry *entry = find_entry(table->entries, table->asize, key);
	if(entry->key==NULL&&entry->value==NULL)table->lsize++; // new, non-tombstone entry has been used
	entry->key=strdup(key);
	entry->value = value;
}

void table_seti(hash_table *table, char *key, int value)
{
	int *i=malloc(sizeof(int));
	*i=value;
	table_set(table,key,i);
}

void *table_get(hash_table *t,char *key)
{
	table_entry *e=find_entry(t->entries,t->asize,key);
	if(e==NULL)return NULL;
	return e->key==NULL?NULL:e->value;
}

void table_del(hash_table *t,char *key)
{
	table_entry *e = find_entry(t->entries,t->asize,key);
	e->key=NULL;
	e->value=(void*)TOMBSTONE;
}

void free_table(hash_table *t, void (*freef)(void *))
{
	for(int i=0;i<t->asize;i++)
	{
		table_entry *e=t->entries+i;
		if(e!=NULL&&e->key!=NULL)
		{
			free(e->key);
			if(freef!=NULL)
			{
				freef(e->value);
			}
		}
	}
	free(t->entries);
}

void init_table(hash_table *t)
{
	t->asize=0;
	t->lsize=0;
	t->entries=NULL;
	adjust_capacity(t,0);
}

hash_table *new_table()
{
	hash_table *t=malloc(sizeof(hash_table));
	init_table(t);
	return t;
}
