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


int expand_path(tokenlist *tokens)
{
    // If nothing exists, return
    if (tokens == NULL || tokens->size ==0)
    {
        return 0;
    }

    // First token is command
    char* command = tokens->items[0];

    // If command already has path, check if it exists
    if (strchr(command, '/') != NULL)
    {
        if (access(command, F_OK) == 0)
        {
            return 1;
        }
        return 0;
    }

    char* path = getenv("PATH");
    if (path == NULL)
    {
        return 0;
    }

    // Copy of path
    char* path_copy = malloc(strlen(path) + 1);
    if (path_copy == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(path_copy, path);

    // First directory from path
    char* directory = strtok(path_copy, ":");

    // Search each directory in path
    while(directory != NULL)
    {
        // Directory + / + command + NULL
        size_t length = strlen(directory) + strlen(command) + 2;

        char* full_path = malloc(length);
        if (full_path == NULL)
        {
            free(path_copy);
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Full command path
        strcpy(full_path, directory);
        strcat(full_path, "/");
        strcat(full_path, command);

        // Check if command exists
        if (access(full_path, F_OK) == 0)
        {
            // Replace command with full path
            free(tokens->items[0]);
            tokens->items[0] = full_path;
            free(path_copy);
            return 1;
        }

        // Path did not work
        free(full_path);
        // Move to next path directory
        directory = strtok(NULL, ":");
    }

    // Command not found
    free(path_copy);
    return 0;
}
        
