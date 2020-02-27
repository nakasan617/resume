CC = gcc
CFLAGS = -O3 -Wall -std=c11
CFLAGS_DEBUG = -Wall -std=c11
DEBUG = -g -ggdb
LIBRARY = -lssl -lcrypto

SOURCES = $(wildcard %.c)
HEADER = $(wildcard %.h)
OBJECTS = $(patsubst %.c, %.o,$(wildcard *.c))
EXECUTABLE = main debug

all: main debug

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBRARY)

debug: $(OBJECTS)
	$(CC) $(CFLAGS_DEBUG) $(DEBUG) main.c parse.c pos.c queue.c node.c hashmap.c -o $@ $(LIBRARY) 

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: clean_all
clean_all:
	rm -f $(OBJECTS) $(EXECUTABLE) posmeta/*
