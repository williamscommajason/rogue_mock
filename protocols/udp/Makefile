CC = gcc
CLFAGS = -Wall -g

all: practice_udp packetsniff

*.o: *.c 
	$(CC) $(CFLAGS) -c $^

client: client.o
	$(CC) $(CFLAGS) $^ -o $@

server: server.o
	$(CC) $(CFLAGS) $^ -o $@

practice_udp: udp.o practice_udp.o
	$(CC) $(CFLAGS) $^ -o $@

packetsniff: packetsniff.o
	$(CC) $(CFLAGS) $^ -o $@
