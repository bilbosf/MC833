# compiler: gcc
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

all: server userdb

server: src/server.c
	$(CC) $(CFLAGS) -o bin/server src/server.c

userdb: src/userdb.c
	$(CC) $(CFLAGS) src/userdb.c -l sqlite3 -o bin/userdb

clean:
	rm -rf bin/*