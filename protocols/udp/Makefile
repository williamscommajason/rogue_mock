CC = gcc
CLFAGS = -Wall -g

all: client server

*.o: *.c 
	$(CC) $(CFLAGS) -c $^

client: client.o
	$(CC) $(CFLAGS) $^ -o $@

server: server.o
	$(CC) $(CFLAGS) $^ -o $@
