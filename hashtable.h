#pragma once

#define MAX_CELLS 512

typedef void (*func_ptr)(struct program_state_s *state);

typedef struct table_chain_int_t
{
    unsigned long int hash;
    int value;
    struct table_chain_int_t *next;
} table_chain_int;

typedef struct table_chain_funcptr_t
{
    unsigned long int hash;
    func_ptr value;
    struct table_chain_funcptr_t *next;
} table_chain_funcptr;

typedef struct
{
    table_chain_int *cells[MAX_CELLS];
    unsigned long int hashes[MAX_CELLS];
} hashtable;

typedef struct
{
    table_chain_funcptr *cells[MAX_CELLS];
    unsigned long int hashes[MAX_CELLS];
} calltable;

hashtable* create_table();
void table_set(hashtable *table, char *key, int value);
int table_get(hashtable *table, char *key);
int table_exists(hashtable *table, char *key);


calltable* create_calltable();
void calltable_set(calltable *table, char *key, func_ptr value);
func_ptr calltable_get(calltable *table, char *key);
int calltable_exists(calltable *table, char *key);