// parts 4-7 imp

#include "executor.h"


static void run_child(char **argv, int in_fd, int out_fd) {
    if (in_fd != STDIN_FILENO) {
      if (dup2(in_fd, STDIN_FILENO) < 0) {
        exit(1);
      }
      close(in_fd);
    }
      
    if (out_fd != STDOUT_FILENO) {
      if (dup2(out_fd, STDOUT_FILENO) < 0) {
        exit(1);
      }
      close(out_fd);
    }

    /* execve from here */
}

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

    /* slice commands at pipes and extract redirections */
    char **cmd_argv[MAX_COMMANDS];
    char *input_file = NULL;
    char *output_file = NULL;
    int cmd_count = 0;

    cmd_argv[cmd_count++] = &tokens->items[0];

    for (size_t i=0; i< tokens->size; i++){
        if (!tokens->items[i]){
          continue;
        }  

        if (strcmp(tokens->items[i], "|") == 0) {
          free(tokens->items[i]);
          tokens->items[i] = NULL;
          if (cmd_count >= MAX_COMMANDS){
            fprintf(stderr, "Error: maximum 2 pipes allowed\n");
            return;
          }

          if (i+1 < tokens->size) {
            cmd_argv[cmd_count++] = &tokens->items[i+1];    
          }
        }

        else if (strcmp(tokens->items[i], "<") == 0){
          if (cmd_count > 1) {
            fprintf(stderr, "Error: input redirection only allowed on first command\n");
            return;
          }
          free(tokens -> items[i]);
          tokens->items[i] = NULL;
          if (i+1 < tokens->size){
            input_file = tokens->items[++i];
          }          
        }

        else if (strcmp(tokens->items[i], ">") == 0){
          free(tokens->items[i]);
          tokens->items[i] = NULL;
          if (i+1 < tokens->size) {
            output_file = tokens->items[++i];
          }
        }
    }

    /* suppress unused warnings until all functionalities are implemented */
    (void)is_bg;
    (void)input_file;
    (void)output_file;
    (void)cmd_argv;
    (void)raw_cmd;
    (void)run_child;
  /* verification, forking, and running pipleline stages */

}
