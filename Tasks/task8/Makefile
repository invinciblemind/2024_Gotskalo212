CC = g++
CCFLAGS = -Wall -std=c++14

PROGRAM = task
OBJECTS = main.o matrix.o

$(PROGRAM): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $(PROGRAM) $(OBJECTS)

main.o: main.cpp matrix.h
	$(CC) $(CCFLAGS) -c main.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) $(CCFLAGS) -c matrix.cpp

clean:
	rm -f $(OBJECTS) $(PROGRAM)

.PHONY: $(PROGRAM)

test:
	valgrind --leak-check=full ./$(PROGRAM)
