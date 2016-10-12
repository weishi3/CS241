/** @file datastore.h */
#ifndef __DS_H__
#define __DS_H__
#include<sys/types.h>
#include<stdint.h>

typedef struct _datastore_t
{
    void * ds;
} datastore_t;


typedef struct _list_t_ {
    char *str;//key
    char *value;
    unsigned long rev;
    struct _list_t_ *next;
} list_t;

typedef struct _hash_table_t_ {
    int size;       /* the size of the table */
    list_t **table; /* the table elements */
} hash_table_t;

hash_table_t *create_hash_table(int size);

unsigned int hash(hash_table_t *hashtable, char *str);

list_t *lookup_string(hash_table_t *hashtable, char *str);

int add_string(hash_table_t *hashtable, char *str,char *value,unsigned long rev);

void free_table(hash_table_t *hashtable);

void update(hash_table_t *hashtable,char *str,char*value);

//void removeele(hash_table_t *hashtable,char *str);

void remove_key(hash_table_t * hashtable, list_t * list, char * key);

#endif
