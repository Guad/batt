#pragma once
#include "interpreter.h"

void init_table();
int try_call(program_state *state, char *name);