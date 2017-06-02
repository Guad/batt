#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"

// max chars that a token can represent
#define MAX_TOKEN_SIZE 30

int is_number(char *str, int len)
{
    for (int i = 0; i < len; ++i)
        if (!isdigit(str[i]) && str[i] != '.')
            return 0;
    return 1;
}

void free_token(token *t)
{
    if (t->value.data != NULL && (t->type == literal_string || t->type == func_call || t->type == quote))
    {
        if (t->type == quote)
            free_token(t->value.data);
        else
            free(t->value.data);
    }

    free(t);
}

token* copy_token(token* origin)
{
    token *output = malloc(sizeof(token));

    output->type = origin->type;
    
    if (origin->type == literal_number)
        output->value.literal = origin->value.literal;
    else if (origin->type == literal_string || origin->type == func_call || origin->type == func_def)
    {
        int len = strlen((char*)origin->value.data) + 1;
        output->value.data = malloc(sizeof(char) * len);        
        memcpy(output->value.data, origin->value.data, len);
    }
    else if (origin->type == quote)
    {
        output->value.data = copy_token((token*)origin->value.data);
    }

    return output;
}


token* create_literal(char *str, int len)
{
    token *t = malloc(sizeof(token));
    t->value.data = NULL;

    if (strncmp(str, "proc", len) == 0)
    {
        t->type = func_def;
    }
    else if (strncmp(str, "end", len) == 0)
    {
        t->type = func_end;
    }
    else if (str[0] == '"' && str[len - 1] == '"') // string
    {
        t->type = literal_string;
        t->value.data = malloc(sizeof(char) * len - 1);
        memcpy(t->value.data, str + 1, len - 1);
        ((char*)t->value.data)[len - 2] = '\0';
    }
    else if (is_number(str, len))
    {
        t->type = literal_number;
        t->value.literal = (float) atof(str);
        // WARN: possible buffer overflow
        // TODO: Write a safer atof
    }
    else
    {
        // Assume it's a func call
        t->type = func_call;
        t->value.data = malloc(sizeof(char) * (len + 1));
        memcpy(t->value.data, str, len);
        ((char*)t->value.data)[len] = '\0';
    }

    return t;
}

token_list* tokenize(char *program, int len, int *token_count)
{
    token_list *root = NULL,
               *currentNode = NULL;
    char buffer[MAX_TOKEN_SIZE];

    int insideComment = 0,
        insideQuote   = 0,
        insideWrap    = 0,
        bufferCounter = 0;

    for (int i = 0; i < len; ++i)
    {
        if (program[i] == '\n' || program[i] == '\r')
        {
            insideComment = 0;
        }

        if (program[i] == ';')
            insideComment = 1;

        if (insideComment) continue;

        if (program[i] == '"')
            insideQuote = !insideQuote;

        if ((program[i] == ' ' || program[i] == '\t' || program[i] == '\n') && !insideQuote)
        {
            if (bufferCounter > 0)
            {
                token *thisToken;

                buffer[bufferCounter] = '\0';

                thisToken = create_literal(buffer, bufferCounter);

                if (insideWrap)
                {
                    token *wrapper = malloc(sizeof(token));
                    wrapper->type = quote;
                    wrapper->value.data = thisToken;
                    thisToken = wrapper;
                }

                token_list *node = malloc(sizeof(token_list));
                node->data = thisToken;
                node->next = NULL;

                if (currentNode != NULL)
                    currentNode->next = node;
                else root = node;
                currentNode = node;
                (*token_count)++;
            }

            bufferCounter = 0;
            insideWrap = 0;
            continue;
        }

        if (program[i] == '@')
            insideWrap++;
        else
            buffer[bufferCounter++] = program[i];
    }

    if (bufferCounter > 0)
    {
        token *thisToken;
        buffer[bufferCounter] = '\0';
        thisToken = create_literal(buffer, bufferCounter);

        if (insideWrap)
        {
            token *wrapper = malloc(sizeof(token));
            wrapper->type = quote;
            wrapper->value.data = thisToken;
            thisToken = wrapper;
        }

        token_list *node = malloc(sizeof(token_list));
        node->data = thisToken;
        node->next = NULL;

        if (currentNode != NULL)
            currentNode->next = node;
        else root = node;
        currentNode = node;
        (*token_count)++;
    }

    return root;
}
