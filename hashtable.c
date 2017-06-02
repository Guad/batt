#include <string.h>
#include <stdlib.h>
#include "hashtable.h"

// Excerpt from https://en.wikipedia.org/wiki/Jenkins_hash_function
unsigned long int jenkins_one_at_a_time_hash(const unsigned char* key, size_t length) {
    size_t i = 0;
    unsigned long int hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

int get_cell(const char *key, unsigned long int *hash)
{
    *hash = jenkins_one_at_a_time_hash(key, strlen(key));
    return  *hash % MAX_CELLS;
}

hashtable* create_table()
{
    hashtable *table = malloc(sizeof(hashtable));
    memset(table, 0, sizeof(hashtable));
    return table;
}

void table_set(hashtable *table, char *key, int value)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    table->cells[cell] = value;
    table->hashes[cell] = hash;
}

int table_get(hashtable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    if (hash != table->hashes[cell])
        return 0; // In case of a hash collision, return default value.
    return table->cells[cell];
}

int table_exists(hashtable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);
    return table->cells[cell] != 0 && table->hashes[cell] == hash;
}

calltable* create_calltable()
{
    calltable *table = malloc(sizeof(calltable));
    memset(table, 0, sizeof(calltable));
    return table;
}

void calltable_set(calltable *table, char *key, func_ptr value)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    table->cells[cell] = value;
    table->hashes[cell] = hash;
}

func_ptr calltable_get(calltable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    if (hash != table->hashes[cell])
        return 0;
    return table->cells[cell];
}

int calltable_exists(calltable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);
    return table->cells[cell] != 0 && table->hashes[cell] == hash;
}