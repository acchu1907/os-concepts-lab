/* Banker's Algorithm: deadlock avoidance.
 * 1) Safety algorithm finds a safe sequence (or reports an unsafe state).
 * 2) Resource-request algorithm grants a request only if the state stays safe.
 * Compile: gcc bankers.c -o bankers */
#include <stdio.h>
#include <string.h>

#define P 5   /* processes */
#define R 3   /* resource types */

int alloc[P][R] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
int max[P][R]   = {{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}};
int avail[R]    = {3,3,2};
int need[P][R];

void compute_need(void) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

int is_safe(int work_in[R], int seq[P]) {
    int work[R], finish[P] = {0}, count = 0;
    memcpy(work, work_in, sizeof(work));
    while (count < P) {
        int found = 0;
        for (int i = 0; i < P; i++) {
            if (finish[i]) continue;
            int ok = 1;
            for (int j = 0; j < R; j++)
                if (need[i][j] > work[j]) { ok = 0; break; }
            if (ok) {
                for (int j = 0; j < R; j++) work[j] += alloc[i][j];
                seq[count++] = i;
                finish[i] = 1;
                found = 1;
            }
        }
        if (!found) return 0;   /* no process can proceed: unsafe */
    }
    return 1;
}

void print_seq(int seq[P]) {
    for (int i = 0; i < P; i++) printf("P%d%s", seq[i], i < P - 1 ? " -> " : "\n");
}

int request(int p, int req[R]) {
    for (int j = 0; j < R; j++) {
        if (req[j] > need[p][j]) { printf("Error: P%d exceeded its maximum claim.\n", p); return 0; }
        if (req[j] > avail[j])   { printf("P%d must wait: resources not available.\n", p); return 0; }
    }
    /* Pretend to allocate, then test safety */
    for (int j = 0; j < R; j++) { avail[j] -= req[j]; alloc[p][j] += req[j]; need[p][j] -= req[j]; }
    int seq[P];
    if (is_safe(avail, seq)) {
        printf("Request by P%d GRANTED. Safe sequence: ", p);
        print_seq(seq);
        return 1;
    }
    /* Roll back */
    for (int j = 0; j < R; j++) { avail[j] += req[j]; alloc[p][j] -= req[j]; need[p][j] += req[j]; }
    printf("Request by P%d DENIED: granting it would lead to an unsafe state.\n", p);
    return 0;
}

int main(void) {
    int seq[P];
    compute_need();
    printf("Initial state check:\n");
    if (is_safe(avail, seq)) { printf("System is SAFE. Safe sequence: "); print_seq(seq); }
    else printf("System is UNSAFE (deadlock possible).\n");

    printf("\nTest 1: P1 requests (1,0,2)\n");
    int r1[R] = {1,0,2};
    request(1, r1);

    printf("\nTest 2: P4 requests (3,3,0)\n");
    int r2[R] = {3,3,0};
    request(4, r2);

    printf("\nTest 3: P0 requests (0,2,0)\n");
    int r3[R] = {0,2,0};
    request(0, r3);
    return 0;
}
