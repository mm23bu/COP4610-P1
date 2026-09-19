// coordination

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

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

                // TESTING PART 3: PATH
                int found = expand_path(tokens);
                if (found)
                {
                    printf("Command found: %s\n", tokens->items[0]);
                }
                else
                {
                    printf("Command not found\n");
                }
            }
            free_tokens(tokens);
        }

        free(input);
    }

    return 0;
}
