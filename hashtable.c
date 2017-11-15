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

    table_chain_int **target, *current;
    current = table->cells[cell];
    target = &table->cells[cell];
    
    while (current != NULL)
    {
        if (current->hash == hash)
        {
            current->value = value;
            return;
        }

        target = &current->next;
        current = current->next;
    }

    table_chain_int *newChain = malloc(sizeof(table_chain_int));
    newChain->next = NULL;
    newChain->value = value;
    newChain->hash = hash;

    *target = newChain;
}

int table_get(hashtable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    table_chain_int *current;
    current = table->cells[cell];

    while (current != NULL)
    {
        if (current->hash == hash)
            return current->value;
        current = current->next;
    }

    return 0;
}

int table_exists(hashtable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    table_chain_int *current;
    current = table->cells[cell];

    while (current != NULL)
    {
        if (current->hash == hash)
            return 1;
        current = current->next;
    }

    return 0;
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

    table_chain_funcptr **target, *current;
    current = table->cells[cell];
    target = &table->cells[cell];

    while (current != NULL)
    {
        if (current->hash == hash)
        {
            current->value = value;
            return;
        }

        target = &current->next;
        current = current->next;
    }

    table_chain_funcptr *newChain = malloc(sizeof(table_chain_funcptr));
    newChain->next = NULL;
    newChain->value = value;
    newChain->hash = hash;

    *target = newChain;
}

func_ptr calltable_get(calltable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    table_chain_funcptr *current;
    current = table->cells[cell];

    while (current != NULL)
    {
        if (current->hash == hash)
            return current->value;
        current = current->next;
    }

    return 0;
}

int calltable_exists(calltable *table, char *key)
{
    unsigned long int hash;
    int cell = get_cell(key, &hash);

    table_chain_funcptr *current;
    current = table->cells[cell];

    while (current != NULL)
    {
        if (current->hash == hash)
            return 1;
        current = current->next;
    }

    return 0;
}