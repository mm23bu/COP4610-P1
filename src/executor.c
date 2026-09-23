// parts 4-7 imp

#include "executor.h"
#include "parser.h"
#include <errno.h>

static void run_child(char **argv, int in_fd, int out_fd) {
    if (in_fd != STDIN_FILENO) {
      if (dup2(in_fd, STDIN_FILENO) < 0) {
        perror("dup2 input");
        _exit(EXIT_FAILURE);
      }
      close(in_fd);
    }
      
    if (out_fd != STDOUT_FILENO) {
      if (dup2(out_fd, STDOUT_FILENO) < 0) {
        perror("dup2 output");
        _exit(EXIT_FAILURE);
      }
      close(out_fd);
    }

    /* execve from here */
    execv(argv[0], argv);

    // Only runs if execv fails
    perror(argv[0]);
    _exit(127);
}

void execute_pipeline(tokenlist *tokens, const char *raw_cmd){
    if (!tokens || tokens->size == 0) {
      return;
    } 

    // Rejects features that are not implemented yet
    for (size_t i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "<") == 0 ||
            strcmp(tokens->items[i], ">") == 0 ||
            strcmp(tokens->items[i], "|") == 0 ||
            strcmp(tokens->items[i], "&") == 0) {
                fprintf(stderr, "Error: redirection, pipes and background not yet supported\n");
                return;
        }
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
  //  (void)cmd_argv;
    (void)raw_cmd;
  //  (void)run_child;
  /* verification, forking, and running pipleline stages */

    // Make sure the command is not empty
    if (cmd_argv[0][0] == NULL || cmd_argv[0][0][0] == '\0') {
        fprintf(stderr, "Command not found\n");
        return;
    }

    // Find the command in PATH
    if (!expand_path(tokens)) {
        fprintf(stderr, "%s: command not found\n", tokens->items[0]);
        return;
    }

    // create pipes
    int pipe_fds[2 * (MAX_COMMANDS - 1)];
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipe_fds + i * 2) < 0) {
            perror ("pipe");
            return;
        }
    }

    // Create child processes
    pid_t pids[MAX_COMMANDS];
    for (int i=0; i< cmd_count; i++) {
        pids[i] = fork();
        if (pids[i] < 0){
            perror("fork");
            return;
        }


        /* decide input source */
        if (pids[i] == 0) {
            int in_fd = STDIN_FILENO;
            int out_fd = STDOUT_FILENO;

            if (i > 0) {
                in_fd = pipe_fds[(i-1) * 2];
            }

            else if (input_file != NULL) {
                in_fd = open(input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror("open input");
                    _exit(EXIT_FAILURE);
                }

            }
        
      
          /* decide output destination */
            if (i < cmd_count - 1) {
              out_fd = pipe_fds[i * 2 + 1];
            }

            else if (output_file != NULL) {
              out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
              if (out_fd < 0) {
                perror("open output");
                _exit(EXIT_FAILURE);
              }
            } 
        
            /* close all pipe copies inherited from the parent */
            for (int j=0; j < 2 * (cmd_count - 1); j++) {
              close(pipe_fds[j]);
            }
            
  
            // Child runs the command
            run_child(cmd_argv[i], in_fd, out_fd);
        }
    }
    
    // Parent waits for the child
   
    for (int i = 0; i < cmd_count; i++) {
      while (waitpid(pids[i], NULL, 0) == -1) {
        if (errno == EINTR) {
            continue;
        }
        perror("waitpid");
        break;
      }
    }

}
