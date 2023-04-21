# compiler: gcc
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

all: client server

client: src/client.c
	$(CC) $(CFLAGS) -o bin/client src/client.c

server: src/server.c
	$(CC) $(CFLAGS) -o bin/server src/server.c

clean:
	rm -rf bin/*