CC = gcc
YACC = yacc

CFLAGS = -Wall -Wextra
YFLAGS = -d

PROGRAMS = prog1 prog2

all: $(PROGRAMS)

prog1: prog1.y
	$(YACC) $(YFLAGS) -o prog1.c prog1.y
	$(CC) $(CFLAGS) prog1.c -o $@

prog2: prog2.y
	$(YACC) $(YFLAGS) -o prog2.c prog2.y
	$(CC) $(CFLAGS) prog2.c -o $@

test1: prog1
	@$(VALGRIND) ./prog1

test2: prog2
	@$(VALGRIND) ./prog2

clean:
	rm -f $(PROGRAMS) *.c *.h $(PROGRAM)

.PHONY: all clean
