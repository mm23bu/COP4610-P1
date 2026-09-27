// parts 8, 9 imp 

#include "jobs.h"
#include <errno.h>

static Job jobs_list[MAX_JOBS];
// static int next_job_id = 1;

static char history[3][MAX_LINE];
static int history_count = 0;

void init_jobs(void) {
    for (int i=0; i < MAX_JOBS; i++){
        jobs_list[i].active = 0;
    }
}

void record_history(const char *cmd_line) {
    if (!cmd_line || cmd_line[0] == '\0') {
        return;
    }

    if (history_count < 3) {
        strncpy(history[history_count], cmd_line, MAX_LINE - 1);
        history[history_count][MAX_LINE - 1] = '\0';
        history_count++;
    }
    
    else {
        strncpy(history[0], history[1], MAX_LINE);
        history[0][MAX_LINE - 1] = '\0';

        strncpy(history[1], history[2], MAX_LINE);
        history[1][MAX_LINE - 1] = '\0';

        strncpy(history[2], cmd_line, MAX_LINE - 1);
        history[2][MAX_LINE - 1] = '\0';

    }
}

void print_history(void) {
    for (int i=0; i < history_count; i++){
        printf("%d: %s\n", i+1, history[i]);
    }
}
