#pragma once

#define MAX_CELLS 512

typedef void (*func_ptr)(struct program_state_s *state);

typedef struct
{
    int cells[MAX_CELLS];
    unsigned long int hashes[MAX_CELLS];
} hashtable;

typedef struct
{
    func_ptr cells[MAX_CELLS];
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