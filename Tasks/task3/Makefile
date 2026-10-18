CC=gcc

CFLAGS=-c -Wall -O0 -g
LDFLAGS=

all: task3

task3_1.o: task3_1.c
	$(CC) $(CFLAGS) task3_1.c -o task3_1.o

task3_f: task3_f
	$(CC) $(CFLAGS) task3_f.c -o task3_f.o

task3: task3_1.o task3_f.o
	$(CC) task3_1.o task3_f.o -o task3

clean:
	rm -rf *.o hello
