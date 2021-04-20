CFLAGS = -O2 -Wall
LDFLAGS = -s
CC = gcc
LD = ld
MAKE = make

CLIENT_CFLAGS=`xmlrpc-c-config  libwww-client --cflags`
CLIENT_LDFLAGS=`xmlrpc-c-config libwww-client --libs`

SERVER_CFLAGS=`xmlrpc-c-config  abyss-server --cflags`
SERVER_LDFLAGS=`xmlrpc-c-config abyss-server --libs`

all: server

server: proces3.c
	$(CC) $(SERVER_CFLAGS)	$(CFLAGS)  -c $<
	$(CC) $(SERVER_LDFLAGS)	$(LDFLAGS) $< -o $@

clean:
	rm -f server *.o 

