/* parts 4, 5, 6, 7 headers*/

#infdef EXECUTOR_H
#define EXECUTOR_H

#include "shell.h"

void execute_pipeline(tokenlist *tokens, const char *raw_cmd, char *raw_input); // raw_cmd = original command string, raw_input = buffer from get_input

#endif
