#include "parser.h"

void expand_tokens(tokenlist *tokens) {
    if (!tokens) {
      return;
    }
  
    for (size_t i = 0; i < tokens->size; i++) {
        char *item = tokens->items[i];

        /* env variables */
        if (item[0] == '$' && strlen(item) > 1) {
            char *var_name = item + 1;
            char *val = getenv(var_name);
            free(tokens->items[i]);
            tokens->items[i] = strdup(val ? val : "");
        }
    }
}
