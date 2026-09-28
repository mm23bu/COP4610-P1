// parts 4-9 imp

#include "executor.h"
#include "parser.h"
#include "jobs.h"
#include <errno.h>

// Close both ends of each pipe that was successfully created
static void close_pipes(int pipe_fds[], int pipe_count) {
    for (int i = 0; i < 2 * pipe_count; i++) {
        close(pipe_fds[i]);
    }
}

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

    execv(argv[0], argv);

    // Only runs if execv fails
    perror(argv[0]);
    _exit(127);
}

// Return a cleaned command line for job display (trim trailing & and spaces)
static void make_display_cmd(const char *raw, char *out, size_t out_sz) {
    if (!raw) {
        out[0] = '\0';
        return;
    }
    strncpy(out, raw, out_sz - 1);
    out[out_sz - 1] = '\0';
    // trim trailing spaces, '&', and spaces again
    size_t len = strlen(out);
    while (len > 0 && (out[len-1] == ' ' || out[len-1] == '\t' || out[len-1] == '\n')) {
        out[len-1] = '\0';
        len--;
    }
    if (len > 0 && out[len-1] == '&') {
        out[len-1] = '\0';
        len--;
        while (len > 0 && (out[len-1] == ' ' || out[len-1] == '\t')) {
            out[len-1] = '\0';
            len--;
        }
    }
}

static int is_builtin(const char *cmd) {
    return (strcmp(cmd, "exit") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd, "jobs") == 0);
}

static void handle_cd(tokenlist *tokens, const char *raw_cmd) {
    // record only on success per "valid commands"
    if (tokens->size == 1) {
        // no argument: change to HOME
        char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
        struct stat st;
        if (stat(home, &st) < 0) {
            fprintf(stderr, "cd: %s: No such file or directory\n", home);
            return;
        }
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "cd: %s: Not a directory\n", home);
            return;
        }
        if (chdir(home) < 0) {
            perror("cd");
            return;
        }
        char cwd[MAX_LINE];
        if (getcwd(cwd, sizeof(cwd))) {
            setenv("PWD", cwd, 1);
        }
        record_history(raw_cmd);
    } else if (tokens->size == 2) {
        const char *target = tokens->items[1];
        struct stat st;
        if (stat(target, &st) < 0) {
            fprintf(stderr, "cd: %s: No such file or directory\n", target);
            return;
        }
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "cd: %s: Not a directory\n", target);
            return;
        }
        if (chdir(target) < 0) {
            perror("cd");
            return;
        }
        char cwd[MAX_LINE];
        if (getcwd(cwd, sizeof(cwd))) {
            setenv("PWD", cwd, 1);
        }
        record_history(raw_cmd);
    } else {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }
}

static void handle_jobs(const char *raw_cmd) {
    print_jobs();
    record_history(raw_cmd);
}

static void handle_exit(const char *raw_cmd) {
    (void)raw_cmd;
    // Reap update before waiting? wait_all will block until done
    // Must wait for background processes to finish
    wait_all_jobs();
    print_history_for_exit();
    // Flush before exit
    fflush(stdout);
    fflush(stderr);
    exit(0);
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

    // Reap completed background jobs before handling next command
    update_jobs();

    /*--- Builtin handling (check before pipe/redirection slicing) ---
    Need to detect builtins before we modify tokens for pipes, but we must also
    detect syntax errors like pipes/redirections with builtins.
    */
    char *first = tokens->items[0];
    if (first && is_builtin(first)) {
        // Check for pipe/redir symbols
        int has_pipe = 0, has_redir = 0;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(tokens->items[i], "|") == 0) has_pipe = 1;
            if (strcmp(tokens->items[i], "<") == 0 || strcmp(tokens->items[i], ">") == 0) has_redir = 1;
            if (strcmp(tokens->items[i], "&") == 0) {
                fprintf(stderr, "Error: invalid background syntax\n");
                return;
            }
        }
        if (has_pipe || has_redir) {
            fprintf(stderr, "Error: builtin commands do not support pipes or redirection\n");
            return;
        }
        if (is_bg) {
            // builtins in background not supported; treat as error or ignore &?
            // For cd/jobs/exit, background makes no sense; we will handle as foreground
            // But spec says background processing supported; we choose to run foreground
        }
        // Make a cleaned raw for history (without &)
        char cleaned[MAX_LINE];
        make_display_cmd(raw_cmd, cleaned, sizeof(cleaned));

        if (strcmp(first, "cd") == 0) {
            handle_cd(tokens, cleaned);
            return;
        } else if (strcmp(first, "jobs") == 0) {
            // jobs should have no args? if args, error but we ignore
            if (tokens->size > 1) {
                fprintf(stderr, "jobs: too many arguments\n");
                return;
            }
            handle_jobs(cleaned);
            return;
        } else if (strcmp(first, "exit") == 0) {
            // exit should have no args; ignore extras
            handle_exit(cleaned);
            return; // not reached
        }
    }

    // Checking syntax before changing original token array (for external commands)
    int has_pipe = 0;
    int has_redirection = 0;
    int pipe_symbols = 0;
    for (size_t i = 0; i < count; i++) {
        char *item = tokens->items[i];
        if (strcmp(item, "&") == 0) {
            fprintf(stderr, "Error: invalid background syntax\n");
            return;
        }
        if (strcmp(item, "|") == 0) {
            has_pipe = 1;
            pipe_symbols++;
            // Max of two pipes
            if (pipe_symbols >= MAX_COMMANDS) {
                fprintf(stderr, "Error: maximum 2 pipes allowed\n");
                return;
            }
            if (i == 0 || i + 1 == count ||
                strcmp(tokens->items[i - 1], "|") == 0 ||
                strcmp(tokens->items[i + 1], "|") == 0) {
                fprintf(stderr, "Error: invalid pipe\n");
                return;
            }
        }
        if (strcmp(item, "<") == 0 || strcmp(item, ">") == 0) {
            has_redirection = 1;
            if (i + 1 == count ||
                strcmp(tokens->items[i + 1], "<") == 0 ||
                strcmp(tokens->items[i + 1], ">") == 0 ||
                strcmp(tokens->items[i + 1], "|") == 0) {
                fprintf(stderr, "Error: missing redirection filename\n");
                return;
            }
        }
    }
    if (has_pipe && has_redirection) {
        fprintf(stderr, "Error: piping with redirection not supported\n");
        return;
    }

    /* slice commands at pipes and extract redirections */
    char **cmd_argv[MAX_COMMANDS];
    char *input_file = NULL;
    char *output_file = NULL;
    int cmd_count = 0;

    cmd_argv[cmd_count++] = &tokens->items[0];

    for (size_t i = 0; i < count; i++) {
        if (!tokens->items[i]) {
            continue;
        }

        if (strcmp(tokens->items[i], "|") == 0) {
            if (cmd_count >= MAX_COMMANDS) {
                fprintf(stderr, "Error: maximum 2 pipes allowed\n");
                return;
            }
            free(tokens->items[i]);
            tokens->items[i] = NULL;  // End arguments for the previous command.
            cmd_argv[cmd_count++] = &tokens->items[i + 1];
        }
        else if (strcmp(tokens->items[i], "<") == 0) {
            if (cmd_count > 1 || input_file != NULL) {
                fprintf(stderr, "Error: invalid input redirection\n");
                return;
            }
            free(tokens->items[i]);
            tokens->items[i] = NULL;
            input_file = tokens->items[++i];  // Keep the filename for open().
        }
        else if (strcmp(tokens->items[i], ">") == 0) {
            if (output_file != NULL) {
                fprintf(stderr, "Error: repeated output redirection\n");
                return;
            }
            free(tokens->items[i]);
            tokens->items[i] = NULL;
            output_file = tokens->items[++i];
        }
    }

    // Prepare cleaned display command for job table / history
    char display_cmd[MAX_LINE];
    make_display_cmd(raw_cmd, display_cmd, sizeof(display_cmd));

   /* verification, forking, and running pipeline stages */

    // Check input file before forking and creating an output file. 
    if (input_file != NULL) {
        struct stat st;
        if (stat(input_file, &st) < 0) {
            perror(input_file);
            return;
        }
        if (!S_ISREG(st.st_mode)) {
            fprintf(stderr, "%s: not a regular file\n", input_file);
            return;
        }
    }

    // PATH search: expects tokenlist*, here is char**, so temp struct, dont free
    for (int i = 0; i < cmd_count; i++) {
        if (cmd_argv[i][0] == NULL || cmd_argv[i][0][0] == '\0') {
            fprintf(stderr, "Command not found\n");
            return;
        }
        tokenlist command;
        command.items = cmd_argv[i];
        command.size = 0;
        while (command.items[command.size] != NULL) {
            command.size++;
        }
        if (!expand_path(&command)) {
            fprintf(stderr, "%s: command not found\n", cmd_argv[i][0]);
            return;
        }
    }

    // create pipes
    int pipe_fds[2 * (MAX_COMMANDS - 1)];
    int pipe_count = 0;
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipe_fds + i * 2) < 0) {
            perror ("pipe");
            close_pipes(pipe_fds, pipe_count);
            return;
        }
        pipe_count++;
    }

    // Create child processes
    pid_t pids[MAX_COMMANDS];
    int created = 0;
    for (int i=0; i< cmd_count; i++) {
        pids[i] = fork();
        if (pids[i] < 0){
            perror("fork");
            break; // Still close pipes and wait for earlier children
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
                // Check the actual file opened by this child.
                struct stat st;
                if (fstat(in_fd, &st) < 0 || !S_ISREG(st.st_mode)) {
                    fprintf(stderr, "Error: input must be a regular file\n");
                    close(in_fd);
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
                if (in_fd != STDIN_FILENO) {
                    close(in_fd);
                }
                _exit(EXIT_FAILURE);
              }
              // open(..., 0600) does not update an existing file's permissions.
              if (fchmod(out_fd, 0600) < 0) {
                perror("fchmod output");
                close(out_fd);
                if (in_fd != STDIN_FILENO) {
                    close(in_fd);
                }
                _exit(EXIT_FAILURE);
              }
            } 
        
            // Close unused pipe ends, keep in_fd and out_fd for run_child()
            for (int j=0; j < 2 * pipe_count; j++) {
                if (pipe_fds[j] != in_fd && pipe_fds[j] != out_fd) {
                    close(pipe_fds[j]);
                }
            }
            // Child runs the command
            run_child(cmd_argv[i], in_fd, out_fd);
        }
        created++;
    }
    
    // Parent closes pipes
    close_pipes(pipe_fds, pipe_count);

    // handle background tracking
    if (is_bg) {
        // Add to jobs table and print
        int job_id = add_job(pids, created, display_cmd);
        if (job_id != -1) {
            // Find display pid from job table (add_job already set)
            // Need to retrieve display pid: for simplicity use pids[1] if pipeline else pids[0]
            pid_t disp = (created > 1) ? pids[1] : pids[0];
            printf("[%d] %d\n", job_id, (int)disp);
            fflush(stdout);
        } else {
            // too many jobs, need to wait and clean up children
            for (int i = 0; i < created; i++) {
                while (waitpid(pids[i], NULL, 0) == -1) {
                    if (errno == EINTR) continue;
                    perror("waitpid");
                    break;
                }
            }
        }
        record_history(display_cmd);
    } else {
        // foreground: wait for all
        for (int i = 0; i < created; i++) {
            int status;
            while (waitpid(pids[i], &status, 0) == -1) {
                if (errno == EINTR) continue;
                perror("waitpid");
                break;
            }
        }
        record_history(display_cmd);
    }
}
