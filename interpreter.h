#pragma once

#include "hashtable.h"
#include "tokens.h"

#define MAX_STACK_SIZE 512

typedef struct program_state_s program_state;

struct program_state_s
{
    token **program;
    int    program_size;

    int    callstack[MAX_STACK_SIZE];
    token* memory[MAX_STACK_SIZE];

    hashtable *calltable;

    int callstack_counter;
    int memory_counter;
    int program_counter;

    int inside_func_def;
};

void execute_token(program_state *state, token *t);
void memory_push(program_state *state, token *data);
token* memory_pop(program_state *state);
void call_func(program_state *state, char *name);
void def_func(program_state *state, char *name, int position);

void abort_program(program_state *state);
void step(program_state *state);
void run(program_state *state);

program_state* init(token_list *tokens, int token_count);
void clean(program_state *state);