#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "functions.h"
#include "interpreter.h"
#include "tokens.h"
#include "tokenizer.h"
#include "hashtable.h"

/* GLOBAL TABLE */

calltable *g_calltable;

/* HELPER */

int digitcount(int n)
{
    int c = 1;
    n /= 10;

    while (n)
    {
        c++;
        n /= 10;
    }

    return c;
}

void ftoa(float n, char *res)
{
	int isnegative = n < 0;
	n = fabs(n);
    int ipart = (int)n;
    float fpart = n - (float)ipart;
 
    int idigits = digitcount(ipart);
    int counter = 0;
    int offset = 0;

    if (isnegative)
    {
        res[0] = '-';
        offset = 1;
    }

    do
    {
        res[idigits - ++counter + offset] = ipart % 10 + '0';
        ipart /= 10;
    } while (ipart);

    if (fpart != 0)
        res[counter++ + offset] = '.';  
    
    while (fpart - (int) fpart > 10e-6) // Only take in account first 6 digits
    {
        fpart *= 10;
        res[counter++ + offset] = ((int)fpart) % 10 + '0';
    }

    res[counter + offset] = '\0';
}

int try_get_number(token *t, float *target)
{
    if (t->type == literal_number)
    {
        *target = t->value.literal;
        return 1;
    }
    else if (t->type == literal_string &&
        is_number(t->value.data, strlen(t->value.data)))
    {
        *target = (float)atof(t->value.data);
        return 1;
    }

    return 0;
}

int try_get_string(token *t, char *target)
{
    if (t->type == literal_string)
    {
        strcpy(target, (char*)t->value.data);
        return 1;
    }
    else if (t->type == literal_number)
    {        
        float num = t->value.literal;
        ftoa(num, target);
        
        return 1;
    }

    return 0;
}

void dump_token(token *t)
{
    char buffer[256];
    if (try_get_string(t, buffer))
    {
        puts(buffer);
    }
}

/* FUNCTIONS */
// Functions are responsible of freeing memory when popping from stack

void sum(program_state *state)
{
    token *right_t = memory_pop(state);
    token *left_t = memory_pop(state);

    float right = 0, left = 0;

    if (try_get_number(right_t, &right) && try_get_number(left_t, &left))
    {
        token *new = malloc(sizeof(token));
        new->type = literal_number;
        new->value.literal = right + left;
        memory_push(state, new);
    }
    else
    {
        printf("Expected a number for sum.\n");
        abort_program(state);
    }

    free_token(right_t);
    free_token(left_t);
}

void sub(program_state *state)
{
    token *right_t = memory_pop(state);
    token *left_t = memory_pop(state);

    float right = 0, left = 0;

    if (try_get_number(right_t, &right) && try_get_number(left_t, &left))
    {
        token *new = malloc(sizeof(token));
        new->type = literal_number;
        new->value.literal = left - right;
        memory_push(state, new);
    }
    else
    {
        printf("Expected a number for sub.\n");
        abort_program(state);
    }

    free_token(right_t);
    free_token(left_t);
}

void mul(program_state *state)
{
    token *right_t = memory_pop(state);
    token *left_t = memory_pop(state);

    float right = 0, left = 0;

    if (try_get_number(right_t, &right) && try_get_number(left_t, &left))
    {
        token *new = malloc(sizeof(token));
        new->type = literal_number;
        new->value.literal = left * right;
        memory_push(state, new);
    }
    else
    {
        printf("Expected a number for mul.\n");
        abort_program(state);
    }

    free_token(right_t);
    free_token(left_t);
}

void divide(program_state *state)
{
    token *right_t = memory_pop(state);
    token *left_t = memory_pop(state);

    float right = 0, left = 0;

    if (try_get_number(right_t, &right) && try_get_number(left_t, &left))
    {
        token *new = malloc(sizeof(token));
        new->type = literal_number;
        new->value.literal = left / right;
        memory_push(state, new);
    }
    else
    {
        printf("Expected a number for div.\n");
        abort_program(state);
    }

    free_token(right_t);
    free_token(left_t);
}

void mod(program_state *state)
{
    token *right_t = memory_pop(state);
    token *left_t = memory_pop(state);

    float right = 0, left = 0;

    if (try_get_number(right_t, &right) && try_get_number(left_t, &left))
    {
        token *new = malloc(sizeof(token));
        new->type = literal_number;
        new->value.literal = (float)((int)left % (int)right);
        memory_push(state, new);
    }
    else
    {
        printf("Expected a number for mod.\n");
        abort_program(state);
    }

    free_token(right_t);
    free_token(left_t);
}

void apply(program_state *state)
{
    token *right_t = memory_pop(state);
    
    execute_token(state, right_t);
}

void print(program_state *state)
{
    token *right_t = memory_pop(state);

    char buffer[256];

    if (try_get_string(right_t, buffer))
    {
        printf(buffer);
    }
    else
    {
        printf("Expected a string or number for print.\n");
        abort_program(state);
    }

    free_token(right_t);
}

void println(program_state *state)
{
    token *right_t = memory_pop(state);

    char buffer[256];

    if (try_get_string(right_t, buffer))
    {
        puts(buffer);
    }
    else
    {
        printf("Expected a string or number for print.\n");
        abort_program(state);
    }

    free_token(right_t);
}

void getln(program_state *state)
{
    char *buffer = malloc(sizeof(char) * 256);
    fgets(buffer, 256, stdin);
    char *newline = strchr(buffer, '\n');
    if (newline)
        *newline = '\0';

    token *newt = malloc(sizeof(token));
    newt->type = literal_string;
    newt->value.data = buffer;

    memory_push(state, newt);
}

void getch(program_state *state)
{
    int c = fgetc(stdin);
    char *str = malloc(sizeof(char) * 2);
    str[0] = c;
    str[1] = '\0';

    token *newt = malloc(sizeof(token));
    newt->type = literal_string;
    newt->value.data = str; 

    memory_push(state, newt);
}

void dup(program_state *state)
{
    token *old = memory_pop(state);

    memory_push(state, old);
    memory_push(state, copy_token(old));
}

void pop(program_state *state)
{
    free_token(memory_pop(state));
}

void swap(program_state *state)
{
    token *right = memory_pop(state);
    token *left = memory_pop(state);

    memory_push(state, right);
    memory_push(state, left);
}

void dip(program_state *state)
{
    token *tmp = memory_pop(state);
    token *quot = memory_pop(state);

    execute_token(state, quot);
    free_token(quot);

    memory_push(state, tmp);
}

void over(program_state *state)
{
    token *tmp = memory_pop(state);
    token *target = memory_pop(state);

    memory_push(state, target);
    memory_push(state, tmp);
    memory_push(state, copy_token(target));
}

void ifelse(program_state *state)
{
    token *second = memory_pop(state);
    token *first = memory_pop(state);
    token *value_t = memory_pop(state);
    float value = 0;
    if (try_get_number(value_t, &value))
    {
        if (value != 0)
            execute_token(state, first);
        else
            execute_token(state, second);
    }
    else
    {
        puts("Expected number for ifelse statement.");
    }

    free_token(first);
    free_token(second);
    free_token(value_t);
}

void cond(program_state *state)
{
    token *first = memory_pop(state);
    token *value_t = memory_pop(state);
    float value = 0;
    if (try_get_number(value_t, &value))
    {
        if (value != 0)
            execute_token(state, first);
    }
    else
    {
        puts("Expected number for if statement.");
    }

    free_token(first);
    free_token(value_t);
}

void equals(program_state *state)
{
    token *first = memory_pop(state);
    token *second = memory_pop(state);
    token *output = malloc(sizeof(token));
    output->type = literal_number;
    output->value.literal = 0;

    float f_first = 0, f_second = 0;

    if (try_get_number(first, &f_first) && try_get_number(second, &f_second))
    {
        if (f_first == f_second)
            output->value.literal = 1;
        else output->value.literal = 0;
    }
    else
    {
        // WARN: Possible buffer overflow
        char buf1[256], buf2[256];
        if (try_get_string(first, buf1) && try_get_string(second, buf2))
        {
            if (!strcmp(buf1, buf2))
                output->value.literal = 1;
            else output->value.literal = 0;
        }
    }

    memory_push(state, output);

    free_token(first);
    free_token(second);
}

void gt(program_state *state)
{
    token *second = memory_pop(state);
    token *first = memory_pop(state);
    token *output = malloc(sizeof(token));

    output->type = literal_number;
    output->value.literal = 0;

    float f_first = 0, f_second = 0;

    if (try_get_number(first, &f_first) && try_get_number(second, &f_second))
    {
        if (f_first > f_second)
            output->value.literal = 1;
        else output->value.literal = 0;
    }
    else
    {
        puts("Expected two numbers for > operator.");
        abort_program(state);
    }

    memory_push(state, output);

    free_token(first);
    free_token(second);
}

void lt(program_state *state)
{
    token *second = memory_pop(state);
    token *first = memory_pop(state);
    token *output = malloc(sizeof(token));

    output->type = literal_number;
    output->value.literal = 0;

    float f_first = 0, f_second = 0;

    if (try_get_number(first, &f_first) && try_get_number(second, &f_second))
    {
        if (f_first < f_second)
            output->value.literal = 1;
        else output->value.literal = 0;
    }
    else
    {
        puts("Expected two numbers for < operator.");
        abort_program(state);
    }

    memory_push(state, output);

    free_token(first);
    free_token(second);
}

void not(program_state *state)
{
    token *first = memory_pop(state);
    token *output = malloc(sizeof(token));

    output->type = literal_number;
    output->value.literal = 1;

    float f_first = 0;

    if (try_get_number(first, &f_first))
    {
        if (f_first == 0)
            output->value.literal = 1;
        else output->value.literal = 0;
    }
    else
    {
        char buf1[256];
        if (try_get_string(first, buf1))
        {
            if (strlen(buf1) == 0)
                output->value.literal = 1;
            else output->value.literal = 0;
        }
    }

    memory_push(state, output);

    free_token(first);
}

void concat(program_state *state)
{
    token *right_t = memory_pop(state);
    token *left_t = memory_pop(state);

    char buffer1[256], buffer2[256];

    if (try_get_string(right_t, buffer2) && try_get_string(left_t, buffer1))
    {
        char *buffer3 = malloc(sizeof(char) * 512);
        strcpy(buffer3, buffer1);
        strcat(buffer3, buffer2);
        
        token *newToken = malloc(sizeof(token));
        newToken->type = literal_string;
        newToken->value.data = buffer3;

        memory_push(state, newToken);
    }
    else
    {
        printf("Expected a string or number for concat.\n");
        abort_program(state);
    }

    free_token(right_t);
    free_token(left_t);
}

void pass(program_state *state)
{
    
}

void dump(program_state *state)
{
    char buffer[256];
    for (int i = 0; i < state->memory_counter; i++)
    {
        if (try_get_string(state->memory[i], buffer))
        {
            printf("%s", buffer);
        }
        printf(", ");
    }

    putchar('\n');
}

void trunk(program_state *state)
{
    token *right_t = memory_pop(state);
    float right;

    if (try_get_number(right_t, &right))
    {
        token *new = malloc(sizeof(token));
        new->type = literal_number;
        new->value.literal = (float) (int) right;

        memory_push(state, new);
    }
    else
    {
        puts("Expected number for trunk");
        abort_program(state);
    }

    free_token(right_t);
}

void init_table()
{
    g_calltable = create_calltable();

    calltable_set(g_calltable, "+", &sum);
    calltable_set(g_calltable, "-", &sub);
    calltable_set(g_calltable, "*", &mul);
    calltable_set(g_calltable, "/", &divide);
    calltable_set(g_calltable, "%", &mod);
    calltable_set(g_calltable, "apply", &apply);
    calltable_set(g_calltable, "print", &print);
    calltable_set(g_calltable, "println", &println);
    calltable_set(g_calltable, "getln", &getln);
    calltable_set(g_calltable, "getch", &getch);
    calltable_set(g_calltable, "dup", &dup);
    calltable_set(g_calltable, "pop", &pop);
    calltable_set(g_calltable, "swap", &swap);
    calltable_set(g_calltable, "dip", &dip);
    calltable_set(g_calltable, "over", &over);
    calltable_set(g_calltable, "ifelse", &ifelse);
    calltable_set(g_calltable, "if", &cond);
    calltable_set(g_calltable, "=", &equals);
    calltable_set(g_calltable, ">", &gt);
    calltable_set(g_calltable, "<", &lt);
    calltable_set(g_calltable, "!", &not);
    calltable_set(g_calltable, "pass", &pass);
    calltable_set(g_calltable, "concat", &concat);
    calltable_set(g_calltable, "dump", &dump);
    calltable_set(g_calltable, "trunk", &trunk);
}

int try_call(program_state *state, char *name)
{
    if (calltable_exists(g_calltable, name))
    {
        (*calltable_get(g_calltable, name))(state);
        return 1;
    }

    return 0;
}