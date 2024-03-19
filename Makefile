CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: bin/main

bin/main: obj/main.o obj/tth.o obj/utils.o obj/merkle_tree.o
	$(CC) $(CFLAGS) obj/main.o obj/tth.o obj/utils.o obj/merkle_tree.o -o bin/main

obj/main.o: src/main.c include/tth.h | obj
	$(CC) $(CFLAGS) -Iinclude -c src/main.c -o obj/main.o

obj/tth.o: src/tth.c include/tth.h | obj
	$(CC) $(CFLAGS) -Iinclude -c src/tth.c -o obj/tth.o

obj/utils.o: src/utils.c include/utils.h | obj
	$(CC) $(CFLAGS) -Iinclude -c src/utils.c -o obj/utils.o

obj/merkle_tree.o: src/merkle_tree.c include/merkle_tree.h | obj
	$(CC) $(CFLAGS) -Iinclude -c src/merkle_tree.c -o obj/merkle_tree.o

obj:
	mkdir -p obj bin

clean:
	rm -rf obj bin
