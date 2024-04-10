CC = gcc
CFLAGS = -Wall -std=c99

OBJS = obj/main.o obj/tth.o obj/utils.o obj/merkle_tree.o

all: bin/main

bin/main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o bin/main -lm

obj/%.o: src/%.c include/tth.h include/merkle_tree.h include/utils.h
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	rm -f obj/* bin/*