CC = gcc
CFLAGS = -Wall -Wextra -O2

all: pc bankers scheduler

pc: producer_consumer.c
	$(CC) $(CFLAGS) producer_consumer.c -o pc -pthread

bankers: bankers.c
	$(CC) $(CFLAGS) bankers.c -o bankers

scheduler: scheduler.c
	$(CC) $(CFLAGS) scheduler.c -o scheduler

clean:
	rm -f pc bankers scheduler
