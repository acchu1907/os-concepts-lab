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

## Concepts demonstrated
Multithreading, semaphores, mutual exclusion, critical sections, deadlock avoidance, safe/unsafe states, CPU scheduling, time complexity trade-offs.
