#pragma once

typedef enum
{
    literal        = 0x01,
    literal_string = 0x03,
    literal_number = 0x05,

    quote          = 0x08,
    func_def       = 0x10,
    func_end       = 0x20,
    func_call      = 0x30,
} token_type;

typedef struct
{
    token_type  type;
    union
    {
        void *data;
        float literal;
    } value;    
} token;

typedef struct token_list token_list;

struct token_list
{
    token *data;
    token_list *next;
};