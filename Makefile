CC=gcc
CFLAGS=-Wall -g
LIBS=-lssl -lcrypto

all: server client1 client2

server: server.o
	$(CC) $(CFLAGS) -o server server.o $(LIBS)

client1: client1.o
	$(CC) $(CFLAGS) -o client1 client1.o $(LIBS)

client2: client2.o
	$(CC) $(CFLAGS) -o client2 client2.o $(LIBS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client1.o: client1.c
	$(CC) $(CFLAGS) -c client1.c

client2.o: client2.c
	$(CC) $(CFLAGS) -c client2.c

clean:
	rm -f *.o server client1 client2
