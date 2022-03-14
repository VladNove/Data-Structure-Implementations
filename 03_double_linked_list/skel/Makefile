CC=clang
CFLAGS=-Wall -Wextra -pedantic
DEBUG=-g -ggdb -O0 -march=native

build: TestCircularDoublyLinkedList TestProblema

TestCircularDoublyLinkedList: src/TestCircularDoublyLinkedList.o src/CircularDoublyLinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) src/TestCircularDoublyLinkedList.o src/CircularDoublyLinkedList.o -o TestCircularDoublyLinkedList

TestProblema: src/TestProblema.o src/CircularDoublyLinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) src/TestProblema.o src/CircularDoublyLinkedList.o -o TestProblema

src/CircularDoublyLinkedList.o: src/CircularDoublyLinkedList.h src/CircularDoublyLinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) src/CircularDoublyLinkedList.c -c -o src/CircularDoublyLinkedList.o

src/TestCircularDoublyLinkedList.o: src/TestCircularDoublyLinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) src/TestCircularDoublyLinkedList.c -c -o src/TestCircularDoublyLinkedList.o

src/TestProblema.o: src/problema2.c
	$(CC) $(CFLAGS) $(DEBUG) src/problema2.c -c -o src/TestProblema.o

run:
	./TestCircularDoublyLinkedList

clean:
	rm src/*.o
	rm TestCircularDoublyLinkedList

