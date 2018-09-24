CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = Assignment2.c

driver: Assignment2.o
	$(CC) $(CFLAGS) -o driver Assignment2.o

Assignment2.o: Assignment2.c
	$(CC) $(CFLAGS) -c Assignment2.c

clean:
	-rm $(OBJECTS)