# Makefile

CC      = gcc
CFLAGS  = -g -Wall

http-client:

.PHONY: clean
clean:
	rm -f *.o a.out http-client

.PHONY: all
all: clean http-client
