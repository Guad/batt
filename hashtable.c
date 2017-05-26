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

int get_cell(const char *key)
{
    return jenkins_one_at_a_time_hash(key, strlen(key)) % MAX_CELLS;
}

hashtable* create_table()
{
    hashtable *table = malloc(sizeof(hashtable));
    memset(table, 0, sizeof(hashtable));
    return table;
}

void table_set(hashtable *table, char *key, int value)
{
    int cell = get_cell(key);

    table->cells[cell] = value;
}

int table_get(hashtable *table, char *key)
{
    int cell = get_cell(key);

    return table->cells[cell];
}

int table_exists(hashtable *table, char *key)
{
    int cell = get_cell(key);
    return table->cells[cell] != 0;
}

calltable* create_calltable()
{
    calltable *table = malloc(sizeof(calltable));
    memset(table, 0, sizeof(calltable));
    return table;
}

void calltable_set(calltable *table, char *key, func_ptr value)
{
    int cell = get_cell(key);

    table->cells[cell] = value;
}

func_ptr calltable_get(calltable *table, char *key)
{
    int cell = get_cell(key);

    return table->cells[cell];
}

int calltable_exists(calltable *table, char *key)
{
    int cell = get_cell(key);
    return table->cells[cell] != 0;
}