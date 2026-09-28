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

void init_jobs(void);

// history
void record_history(const char *cmd_line);
void print_history(void);
void print_history_for_exit(void);
int get_history_count(void);

// jobs
int add_job(pid_t *pids, int num_pids, const char *cmd_line);
void update_jobs(void);
void print_jobs(void);
void wait_all_jobs(void);
int any_jobs_active(void);

#endif
