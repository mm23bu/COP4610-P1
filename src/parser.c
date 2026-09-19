#include "parser.h"

void expand_tokens(tokenlist *tokens) {
    if (!tokens) {
      return;
    }
  
    for (size_t i = 0; i < tokens->size; i++) {
        char *item = tokens->items[i];

        //check for ~ or ~/ pattern
        if (item[0] == '~' && (item[1] == '\0' || item[1] == '/')){
            char *home = getenv("HOME");
            if (home){
                //allocate space for string to include home + path
                char *tilde_expanded = malloc(strlen(home) + strlen(item));
                strcpy(tilde_expanded, home);
                strcat(tilde_expanded, item+1);
                free(tokens->items[i]);
                tokens->items[i] = tilde_expanded;
                item = tilde_expanded;
            }
        }

        /* env variables */
        if (item[0] == '$' && strlen(item) > 1) {
            char *var_name = item + 1;
            char *val = getenv(var_name);
            free(tokens->items[i]);
            tokens->items[i] = strdup(val ? val : "");
        }
    }
}
