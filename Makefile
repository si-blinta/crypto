CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: bin/main

bin/main: obj/main.o obj/tth.o
	$(CC) $(CFLAGS) $^ -o $@

obj/main.o: src/main.c include/tth.h | obj
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

obj/tth.o: src/tth.c include/tth.h | obj
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

obj:
	mkdir -p obj bin

clean:
	rm -rf obj bin
