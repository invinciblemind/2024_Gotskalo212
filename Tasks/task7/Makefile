CC=gcc

CFLAGS=-c -Wall -O0 -g
LDFLAGS=

all: chat2_server chat2_client

chat2_server.o: chat2_server.c
	$(CC) $(CFLAGS) chat2_server.c -o chat2_server.o

chat2_client.o: chat2_client.c
	$(CC) $(CFLAGS) chat2_client.c -o chat2_client.o

chat2_server: chat2_server.o
	$(CC) chat2_server.o -o chat2_server

chat2_client: chat2_client.o
	$(CC) chat2_client.o -o chat2_client

clean:
	rm -rf *.o hello
