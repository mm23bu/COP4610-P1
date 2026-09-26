// parts 8, 9 imp 

#include "jobs.h"

typedef struct {
    int job_id;
    pid_t pids[MAX_COMMANDS];
    int num_pids;
    pid_t display_pid;
    char cmd_line[MAX_LINE];
    int active;
} Job;

Job jobs_list[MAX_JOBS];
int next_job_id = 1;

char history[3][MAX_LINE];
int history_count = 0;

void record_history(const char *cmd_line) {
    if (history_count < 3) {
        strncpy(history[history_count], cmd_line, MAX_LINE - 1);
        history[history_count][MAX_LINE - 1] = '\0';
        history_count++;
    }
    
    else {
        strncpy(history[0], history[1], MAX_LINE);
        strncpy(history[1], history[2], MAX_LINE);
        strncpy(history[2], cmd_line, MAX_LINE - 1);
        history[2][MAX_LINE - 1] = '\0';

    }
}
