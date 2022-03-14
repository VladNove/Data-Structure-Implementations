CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99
DEBUG=-g -ggdb -O0 -march=native

build: Test Paranteze

Test: src/Test.o src/Queue.o src/Stack.o src/LinkedList.o src/QueStack.o
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@

Paranteze: src/Paranteze.o src/Stack.o src/LinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@

src/Queue.o: src/Queue.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

src/Stack.o: src/Stack.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

src/QueStack.o: src/QueStack.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

src/LinkedList.o: src/LinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

src/Test.o: src/Test.c
	$(CC) $(CFLAGS) $(DEBUG) src/Test.c -c -o src/Test.o

src/Paranteze.o: src/Paranteze.c
	$(CC) $(CFLAGS) $(DEBUG) src/Paranteze.c -c -o src/Paranteze.o

run:
	./Test

clean:
	rm -f src/*.o Test Paranteze