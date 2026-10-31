/* CPU Scheduling simulator: FCFS, SJF (non-preemptive), Round Robin.
 * Prints per-process waiting/turnaround time and the averages.
 * Compile: gcc scheduler.c -o scheduler */
#include <stdio.h>

#define N 5
#define QUANTUM 3

int pid[N] = {1,2,3,4,5};
int at[N]  = {0,1,2,3,4};   /* arrival time */
int bt[N]  = {8,4,9,5,2};   /* burst time */

void report(const char *name, int ct[N]) {
    double tw = 0, tt = 0;
    printf("\n=== %s ===\n", name);
    printf("PID  Arrival  Burst  Completion  Turnaround  Waiting\n");
    for (int i = 0; i < N; i++) {
        int tat = ct[i] - at[i];
        int wt = tat - bt[i];
        tw += wt; tt += tat;
        printf("P%-3d %-8d %-6d %-11d %-11d %d\n", pid[i], at[i], bt[i], ct[i], tat, wt);
    }
    printf("Average turnaround = %.2f, Average waiting = %.2f\n", tt / N, tw / N);
}

void fcfs(void) {
    int ct[N], t = 0;
    for (int i = 0; i < N; i++) {          /* already sorted by arrival */
        if (t < at[i]) t = at[i];
        t += bt[i];
        ct[i] = t;
    }
    report("FCFS", ct);
}

void sjf(void) {
    int ct[N], done[N] = {0}, t = 0, count = 0;
    while (count < N) {
        int pick = -1;
        for (int i = 0; i < N; i++)
            if (!done[i] && at[i] <= t && (pick == -1 || bt[i] < bt[pick])) pick = i;
        if (pick == -1) { t++; continue; } /* CPU idle */
        t += bt[pick];
        ct[pick] = t;
        done[pick] = 1;
        count++;
    }
    report("SJF (non-preemptive)", ct);
}

void round_robin(void) {
    int ct[N], rem[N], inq[N] = {0}, q[1000], head = 0, tail = 0, t = 0, done = 0;
    for (int i = 0; i < N; i++) rem[i] = bt[i];
    while (done < N) {
        for (int i = 0; i < N; i++)
            if (at[i] <= t && !inq[i] && rem[i] > 0) { q[tail++] = i; inq[i] = 1; }
        if (head == tail) { t++; continue; }
        int i = q[head++];
        int run = rem[i] < QUANTUM ? rem[i] : QUANTUM;
        t += run;
        rem[i] -= run;
        for (int j = 0; j < N; j++)        /* admit processes that arrived meanwhile */
            if (at[j] <= t && !inq[j] && rem[j] > 0) { q[tail++] = j; inq[j] = 1; }
        if (rem[i] > 0) q[tail++] = i;
        else { ct[i] = t; done++; }
    }
    report("Round Robin (quantum = 3)", ct);
}

int main(void) {
    fcfs();
    sjf();
    round_robin();
    return 0;
}
