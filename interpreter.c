#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "interpreter.h"
#include "hashtable.h"
#include "functions.h"
#include "tokens.h"
#include "tokenizer.h"

void memory_push(program_state *state, token *data)
{
    state->memory[state->memory_counter++] = data;
}

token* memory_pop(program_state *state)
{
    token* t = state->memory[--state->memory_counter];
    state->memory[state->memory_counter] = NULL;
    return t;
}

void callstack_push(program_state *state, int value)
{
    state->callstack[state->callstack_counter++] = value;    
}

int callstack_pop(program_state *state)
{
    return state->callstack[--state->callstack_counter];
}

// Init a program from a list of tokens.
// The linked list is then deleted (but not the contents)
program_state* init(token_list *tokens, int token_count)
{
    program_state *state = malloc(sizeof(program_state));
    memset(state, 0, sizeof(program_state));

    state->program_size = token_count;
    state->program = malloc(sizeof(token*) * token_count);   

    state->calltable = create_table();

    token_list *currentNode = tokens;

    while (currentNode != NULL)
    {
        state->program[state->program_counter++] = currentNode->data;

        token_list *tmp = currentNode;
        currentNode = currentNode->next;
        free(tmp);
    }

    state->program_counter = 0;

    return state;
}

int contains(void **array, int len, void *target)
{
    for (int i = 0; i < len; i++)
        if (array[i] == target)
            return 1;
    return 0;
}

// Clean up the program's memory.
void clean(program_state *state)
{
    void** pointers = malloc(sizeof(void*) * state->program_size + state->memory_counter);
    int sharedcounter = 0;

    for (int i = 0; i < state->program_size; ++i)
    {   
        if (!contains(pointers, sharedcounter, state->program[i]))
        {
            free_token(state->program[i]);
            pointers[sharedcounter++] = state->program[i];
        }
    }

    for (int i = 0; i < state->memory_counter; ++i)
    {
        if (!contains(pointers, sharedcounter, state->memory[i]))
        {
            free_token(state->memory[i]);
            pointers[sharedcounter++] = state->memory[i];
        }
    }
    
    free(state->program);
    free(state->calltable);

    free(state);
}

// Run to the end of program
void run(program_state *state)
{
    for (; state->program_counter < state->program_size;)
    {
        step(state);
    }
}

// Execute one instruction
void step(program_state *state)
{
    execute_token(state, state->program[state->program_counter++]);
}

// abort_program execution
void abort_program(program_state *state)
{
    state->program_counter = INT_MAX;
}

void execute_token(program_state *state, token *t)
{
    if (state->inside_func_def)
    {
        if (t->type == func_end)
            state->inside_func_def = 0;
        return;
    }

    switch (t->type)
    {
    case quote:
        ;
        token *quotedToken = copy_token((token*)t->value.data);
        memory_push(state, quotedToken);
        break;
    case func_def:
        state->inside_func_def = 1;        
        char *name = (char*)state->program[state->program_counter++]->value.data;
        def_func(state, name, state->program_counter);
        break;
    case func_end:
        state->program_counter = callstack_pop(state);
        break;
    case func_call:
        ;
        char *fname = (char*) t->value.data;
        call_func(state, fname);
        break;
    case literal:
    case literal_string:
    case literal_number:
        memory_push(state, copy_token(t));
        break;
    }
}

void call_func(program_state *state, char *name)
{
    if (!try_call(state, name))
    {
        int pos = table_get(state->calltable, name);

        if (pos)
        {
            callstack_push(state, state->program_counter);
            state->program_counter = pos;
        }
        else
        {
            printf("ERROR: No such function: %s\n", name);
            abort_program(state);
        }
    }
}

void def_func(program_state *state, char *name, int pos)
{
    table_set(state->calltable, name, pos);
}