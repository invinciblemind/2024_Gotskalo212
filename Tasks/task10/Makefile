CC = g++
CCFLAGS = -Wall -std=c++14

PROGRAM = task10
OBJECTS = lex_analyze.o lex_analyze_functions.o syntax_analyze.o
HEADERS = lex_analyze.h

$(PROGRAM): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $(PROGRAM) $(OBJECTS)

lex_analyze.o: lex_analyze.cpp $(HEADERS)
	$(CC) $(CCFLAGS) -c lex_analyze.cpp

lex_analyze_functions.o: lex_analyze_functions.cpp $(HEADERS)
	$(CC) $(CCFLAGS) -c lex_analyze_functions.cpp

syntax_analyze.o: syntax_analyze.cpp $(HEADERS)
	$(CC) $(CCFLAGS) -c syntax_analyze.cpp

clean:
	rm -f $(OBJECTS) $(PROGRAM)

.PHONY: $(PROGRAM)

test:
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROGRAM)
