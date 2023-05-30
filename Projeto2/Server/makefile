# compiler: gcc
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

all: server userdb images

server: src/server.c src/userdb.c src/server_send.c
	$(CC) $(CFLAGS) -o bin/server src/server.c src/userdb.c src/server_send.c -l sqlite3

userdb: src/userdb.c src/images.c
	$(CC) $(CFLAGS) src/userdb.c src/images.c -l sqlite3 -o bin/userdb

images: src/images.c
	$(CC) $(CFLAGS) src/images.c -o bin/images

clean:
	rm -rf bin/*