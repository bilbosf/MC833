# compiler: gcc
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

all: client

client: src/client.c
	$(CC) $(CFLAGS) -o bin/client src/client.c src/netservice.c

clean:
	rm -rf bin/*