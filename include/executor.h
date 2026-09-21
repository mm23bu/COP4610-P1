/* parts 4, 5, 6, 7 headers*/

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "shell.h"

void execute_pipeline(tokenlist *tokens, const char *raw_cmd); // raw_cmd = original command string

#endif
