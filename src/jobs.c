// parts 8, 9 imp

#include "jobs.h"
#include <errno.h>

static Job jobs_list[MAX_JOBS];
static int next_job_id = 1;

static char history[3][MAX_LINE];
static int history_count = 0;

void init_jobs(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs_list[i].active = 0;
        jobs_list[i].job_id = 0;
    }
    next_job_id = 1;
    history_count = 0;
}

void record_history(const char *cmd_line) {
    if (!cmd_line || cmd_line[0] == '\0') {
        return;
    }

    if (history_count < 3) {
        strncpy(history[history_count], cmd_line, MAX_LINE - 1);
        history[history_count][MAX_LINE - 1] = '\0';
        history_count++;
    } else {
        strncpy(history[0], history[1], MAX_LINE);
        history[0][MAX_LINE - 1] = '\0';

        strncpy(history[1], history[2], MAX_LINE);
        history[1][MAX_LINE - 1] = '\0';

        strncpy(history[2], cmd_line, MAX_LINE - 1);
        history[2][MAX_LINE - 1] = '\0';
    }
}

void print_history(void) {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

int get_history_count(void) {
    return history_count;
}

void print_history_for_exit(void) {
    if (history_count == 0) {
        printf("No valid commands\n");
    } else if (history_count < 3) {
        // If there were less than three valid commands, print the last valid one.
        printf("%s\n", history[history_count - 1]);
    } else {
        for (int i = 0; i < 3; i++) {
            printf("%s\n", history[i]);
        }
    }
    fflush(stdout);
}

int add_job(pid_t *pids, int num_pids, const char *cmd_line) {
    int slot = -1;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs_list[i].active) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        fprintf(stderr, "Error: too many background jobs\n");
        return -1;
    }
    jobs_list[slot].job_id = next_job_id++;
    jobs_list[slot].num_pids = num_pids;
    for (int i = 0; i < num_pids; i++) {
        jobs_list[slot].pids[i] = pids[i];
    }
    // display PID: for pipeline use second pid if available, else first
    if (num_pids > 1) {
        jobs_list[slot].display_pid = pids[1];
    } else if (num_pids > 0) {
        jobs_list[slot].display_pid = pids[0];
    } else {
        jobs_list[slot].display_pid = -1;
    }
    strncpy(jobs_list[slot].cmd_line, cmd_line ? cmd_line : "", MAX_LINE - 1);
    jobs_list[slot].cmd_line[MAX_LINE - 1] = '\0';
    jobs_list[slot].active = 1;
    return jobs_list[slot].job_id;
}

void update_jobs(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs_list[i].active) continue;
        int all_done = 1;
        for (int j = 0; j < jobs_list[i].num_pids; j++) {
            pid_t pid = jobs_list[i].pids[j];
            if (pid <= 0) continue;
            int status;
            pid_t ret = waitpid(pid, &status, WNOHANG);
            if (ret == 0) {
                // still running
                all_done = 0;
            } else if (ret == -1) {
                if (errno == ECHILD) {
                    // already reaped, consider done
                    continue;
                } else {
                    // error, consider still active? but treat as done
                    continue;
                }
            } else {
                // ret == pid, process terminated, keep checking others
                // mark this pid as done (set to 0 so we don't wait again)
                // but keep count; we will mark job inactive if all done
                jobs_list[i].pids[j] = 0;
            }
        }
        // second pass: if any pid still >0 and still running, not all_done
        // we already set all_done based on WNOHANG results, but after reaping some,
        // need to check remaining pids that are still >0
        // If all pids became 0 or ECHILD, job is done
        if (all_done) {
            // verify no remaining active pid
            int any_remaining = 0;
            for (int j = 0; j < jobs_list[i].num_pids; j++) {
                if (jobs_list[i].pids[j] > 0) {
                    // we had ret==0 for this pid above, so still running
                    any_remaining = 1;
                    break;
                }
            }
            if (!any_remaining) {
                jobs_list[i].active = 0;
            }
        }
    }
}

void print_jobs(void) {
    update_jobs();
    int any = 0;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs_list[i].active) {
            any = 1;
            printf("[%d]+ %d %s\n", jobs_list[i].job_id, (int)jobs_list[i].display_pid, jobs_list[i].cmd_line);
        }
    }
    if (!any) {
        printf("No active background processes\n");
    }
    fflush(stdout);
}

void wait_all_jobs(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs_list[i].active) continue;
        for (int j = 0; j < jobs_list[i].num_pids; j++) {
            pid_t pid = jobs_list[i].pids[j];
            if (pid <= 0) continue;
            int status;
            while (waitpid(pid, &status, 0) == -1) {
                if (errno == EINTR) continue;
                if (errno == ECHILD) break;
                perror("waitpid");
                break;
            }
        }
        jobs_list[i].active = 0;
    }
}

int any_jobs_active(void) {
    update_jobs();
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs_list[i].active) return 1;
    }
    return 0;
}
