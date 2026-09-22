// coordination

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "executor.h"

int main(void) {
    while (1) {
        print_prompt();

        char *input = get_input();
        if (input == NULL) {
            printf("\n");
            break;
        }

        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        tokenlist *tokens = get_tokens(input);

        if (tokens != NULL) {
            if (tokens->size > 0) {
                expand_tokens(tokens);

                // Execute command
                execute_pipeline(tokens, input);
            }
            free_tokens(tokens);
        }

        free(input);
    }

    return 0;
}
