CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = Assignment1.c

driver: Assignment1.o
	$(CC) $(CFLAGS) -o driver Assignment1.o

Assignment1.o: Assignment1.c
	$(CC) $(CFLAGS) -c Assignment1.c

clean:
	-rm $(OBJECTS) driver