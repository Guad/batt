#pragma once
#include "tokens.h"

int is_number(char *str, int len);
token* create_literal(char *str, int len);
token_list* tokenize(char *program, int len, int *token_count);
token* copy_token(token *origin);
void free_token(token *t);