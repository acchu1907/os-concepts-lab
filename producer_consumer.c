/* Producer-Consumer problem using POSIX threads and semaphores.
 * Bounded buffer: producers wait when full, consumers wait when empty,
 * a mutex protects the buffer. Compile: gcc producer_consumer.c -o pc -pthread */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty_slots;   /* counts free slots */
sem_t full_slots;    /* counts filled slots */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = id * 100 + i;
        sem_wait(&empty_slots);              /* wait for a free slot */
        pthread_mutex_lock(&lock);
        buffer[in] = item;
        printf("Producer %d produced %d at slot %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&full_slots);               /* signal item available */
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    int to_consume = ITEMS_PER_PRODUCER * NUM_PRODUCERS / NUM_CONSUMERS;
    for (int i = 0; i < to_consume; i++) {
        sem_wait(&full_slots);               /* wait for an item */
        pthread_mutex_lock(&lock);
        int item = buffer[out];
        printf("    Consumer %d consumed %d from slot %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&empty_slots);              /* signal slot free */
    }
    return NULL;
}

int main(void) {
    pthread_t prod[NUM_PRODUCERS], cons[NUM_CONSUMERS];
    int pid[NUM_PRODUCERS], cid[NUM_CONSUMERS];

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pid[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &pid[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cid[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cid[i]);
    }
    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(cons[i], NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    printf("\nAll items produced and consumed with no race conditions.\n");
    return 0;
}
