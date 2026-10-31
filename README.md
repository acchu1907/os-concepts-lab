# OS Concepts Lab

Small C programs that implement core Operating Systems concepts: process synchronization, deadlock avoidance and CPU scheduling. Built on Linux using POSIX threads.

## Programs

| File | Concept | Details |
|---|---|---|
| `producer_consumer.c` | Synchronization | Bounded-buffer producer-consumer using POSIX threads, counting semaphores (`empty`, `full`) and a mutex. Multiple producers and consumers, no race conditions. |
| `bankers.c` | Deadlock avoidance | Banker's algorithm: safety algorithm to find a safe sequence, and resource-request algorithm that grants a request only if the system stays safe. |
| `scheduler.c` | CPU scheduling | FCFS, non-preemptive SJF and Round Robin with arrival times. Reports turnaround and waiting time per process and averages. |

## Build and run

```bash
make            # builds all three
./pc
./bankers
./scheduler
make clean
```

Requires `gcc` and `make` (Linux, macOS or WSL).

## Sample result (scheduler)

| Algorithm | Avg waiting time | Avg turnaround time |
|---|---|---|
| FCFS | 11.40 | 17.00 |
| SJF | 8.20 | 13.80 |
| Round Robin (q=3) | 14.60 | 20.20 |

SJF gives the lowest average waiting time on this workload, as expected from theory.

# My outputs
# QUANTUM 1

=== FCFS ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      12          11          7
P3   2        9      21          19          10
P4   3        5      26          23          18
P5   4        2      28          24          22
Average turnaround = 17.00, Average waiting = 11.40

=== SJF (non-preemptive) ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      14          13          9
P3   2        9      28          26          17
P4   3        5      19          16          11
P5   4        2      10          6           4
Average turnaround = 13.80, Average waiting = 8.20

=== Round Robin (quantum = 1) ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      25          25          17
P2   1        4      15          14          10
P3   2        9      28          26          17
P4   3        5      22          19          14
P5   4        2      13          9           7
Average turnaround = 18.60, Average waiting = 13.00

# QUANTUM 5

=== FCFS ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      12          11          7
P3   2        9      21          19          10
P4   3        5      26          23          18
P5   4        2      28          24          22
Average turnaround = 17.00, Average waiting = 11.40

=== SJF (non-preemptive) ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      14          13          9
P3   2        9      28          26          17
P4   3        5      19          16          11
P5   4        2      10          6           4
Average turnaround = 13.80, Average waiting = 8.20

=== Round Robin (quantum = 5) ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      24          24          16
P2   1        4      9           8           4
P3   2        9      28          26          17
P4   3        5      19          16          11
P5   4        2      21          17          15
Average turnaround = 18.20, Average waiting = 12.60

# QUANTUM 20 
=== FCFS ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      12          11          7
P3   2        9      21          19          10
P4   3        5      26          23          18
P5   4        2      28          24          22
Average turnaround = 17.00, Average waiting = 11.40

=== SJF (non-preemptive) ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      14          13          9
P3   2        9      28          26          17
P4   3        5      19          16          11
P5   4        2      10          6           4
Average turnaround = 13.80, Average waiting = 8.20

=== Round Robin (quantum = 20) ===
PID  Arrival  Burst  Completion  Turnaround  Waiting
P1   0        8      8           8           0
P2   1        4      12          11          7
P3   2        9      21          19          10
P4   3        5      26          23          18
P5   4        2      28          24          22
Average turnaround = 17.00, Average waiting = 11.40

## My experiments

### Round Robin quantum comparison
| Quantum | Avg waiting | Avg turnaround |
|---|---|---|
| 1 | 13.00 | 18.60 |
| 3 | 14.60 | 20.20 |
| 5 | 12.60 | 18.20 |
| 20 | 11.40 | 17.00 |

- At quantum 20 (larger than every burst time) Round Robin gives exactly the same results as FCFS.
- Averages are not monotonic in the quantum: quantum 3 was the worst case on this workload.
- SJF stayed lowest (8.20 avg waiting) in every run.
- The simulator ignores context-switch overhead, which would penalise very small quanta in a real system.

### Deadlock experiment
- Reversed the lock order in the producer (mutex before `sem_wait`) and slowed the consumers with `usleep`. The program froze once the buffer filled, because the producer held the mutex while waiting for a free slot and the consumers needed that mutex to free one.
- Restored the correct order (semaphore first, then mutex), and the program completes normally.

### Banker's algorithm extra tests
- Test 4: P3 requests (0,1,0). Granted, system stays safe.
- Test 5: P3 requests (0,2,0). Rejected with "exceeded maximum claim", since P3's remaining need after Test 4 is (0,0,1).
- Results carry over between tests: a granted request changes the state for later requests, while a denied one is rolled back.

## Concepts demonstrated
Multithreading, semaphores, mutual exclusion, critical sections, deadlock avoidance, safe/unsafe states, CPU scheduling, time complexity trade-offs.
