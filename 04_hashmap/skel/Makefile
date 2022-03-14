CC=clang
CFLAGS=-Wall -Wextra -pedantic
DEBUG=-g -ggdb -O0 -march=native

build: TestHashtable Adrese

TestHashtable: src/TestHashtable.o src/Hashtable_unlinked.o src/LinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) src/TestHashtable.o src/Hashtable_unlinked.o src/LinkedList.o -o TestHashtable

Adrese: src/Adrese.o src/Hashtable_unlinked.o src/LinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) src/Adrese.o src/Hashtable_unlinked.o src/LinkedList.o -o Adrese


src/Hashtable_unlinked.o: src/Hashtable.h src/Hashtable.c
	$(CC) $(CFLAGS) $(DEBUG) src/Hashtable.c -c -o src/Hashtable_unlinked.o

src/LinkedList.o: src/LinkedList.h src/LinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) src/LinkedList.c -c -o src/LinkedList.o

src/TestHashtable.o: src/TestHashtable.c
	$(CC) $(CFLAGS) $(DEBUG) src/TestHashtable.c -c -o src/TestHashtable.o

src/Adrese.o: src/Adrese.c
	$(CC) $(CFLAGS) $(DEBUG) src/Adrese.c -c -o src/Adrese.o

run:
	./TestHashtable

clean:
	rm src/*.o
	rm TestHashtable
	rm Adrese

