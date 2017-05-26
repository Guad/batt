#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "interpreter.h"
#include "functions.h"

int main(int argc, char *argv[])
{
    FILE *script;
	char *path;

	init_table();
    
	if (argc <= 1)
    {
#ifndef _DEBUG
        puts("USAGE: battc [path to script]\n");
        return 0;
#else
		path = "test.batt";
#endif
    }
	else path = argv[1];

    if ((script = fopen(path, "r")))
    {
        char buffer[10 * 1024] = { 0 };
        int c, i = 0;

        while ((c = fgetc(script)) != EOF)
        {
            buffer[i++] = c;
        }

        int tokenCount = 0;
        
        token_list *tokens = tokenize(buffer, i, &tokenCount);

        program_state *program = init(tokens, tokenCount);

        run(program);
        clean(program);
		//free(program);
        program = NULL;
		fclose(script);
    }
    else
    {
        printf("Cannot open file %s", path);
    }


    return 0;
}