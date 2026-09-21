/* shared defs and structs */

#ifndef SHELL_H
#define SHELL_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lexer.h"

#define MAX_LINE 1024
#define MAX_COMMANDS 3
#define MAX_JOBS 10

void record_history(const char *cmd_line);

#endif
