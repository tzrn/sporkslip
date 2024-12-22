#ifndef HASHTABLE_H
#define HASHTABLE_H

#define TABLE_MAX_LOAD  0.5
#define TABLE_INIT_SIZE 20
#define TOMBSTONE       -1

typedef struct
{
	char *key;
	void *value;
} table_entry;

typedef struct table
{
	int asize;
	int lsize;
	table_entry *entries;
} hash_table;

table_entry *find_entry(table_entry *entries, int asize,char *key);

void table_set(hash_table *table, char *key, void *value);
void table_seti(hash_table *table, char *key, int value);
void *table_get(hash_table *t,char *key);
void table_del(hash_table *t,char *key);
void free_table(hash_table *t, void (*freef)(void *));

void init_table(hash_table *t);
hash_table *new_table();

#endif
