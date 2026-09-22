/* parts 0, 2, 3 */

#ifndef PARSER_H
#define PARSER_H

#include "shell.h"

/* in-place token modifications */
void expand_tokens(tokenlist *tokens);

// Finds the command using PATH
int expand_path(tokenlist *tokens);

#endif
