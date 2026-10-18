CC=gcc

CFLAGS=-c -Wall -O0 -g

all: task5

task3_1.o: task3_1.c
	$(CC) $(CFLAGS) task3_1.c -o task3_1.o

task3_f.o: task3_f.c
	$(CC) $(CFLAGS) task3_f.c -o task3_f.o

task5_tree.o: task5_tree.c
	$(CC) $(CFLAGS) task5_tree.c -o task5_tree.o
	
launch_tree.o: launch_tree.c
	$(CC) $(CFLAGS) launch_tree.c -o launch_tree.o

task5: task3_1.o task3_f.o task5_tree.o launch_tree.o
	$(CC) task3_1.o task3_f.o task5_tree.o launch_tree.o -o task5

clean:
	rm -rf *.o hello
