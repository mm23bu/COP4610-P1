// parts 4-7 imp

#include "executor.h"


void execute_pipeline(tokenlist *tokens, const char *raw_cmd){
    if (!tokens || tokens->size == 0) {
      return;
    } 

    size_t count = tokens->size;

    int is_bg = 0;

    // check for trailing '&', which leads to background process
    if (count > 0 && strcmp(tokens->items[count - 1], "&") == 0){
      is_bg = 1;
      free(tokens->items[count-1]);
      tokens->items[count - 1] = NULL;
      tokens->size--;
      count--;
    }

    if (count == 0){
      return;
    }
    
}
