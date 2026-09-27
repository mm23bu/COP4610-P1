/* parts 8, 9 */

#ifndef JOBS_H
#define JOBS_H

#include "shell.h"

typedef struct {
    int job_id;
    pid_t pids[MAX_COMMANDS];
    int num_pids;
    pid_t display_pid;
    char cmd_line[MAX_LINE];
    int active;
} Job;

#endif
