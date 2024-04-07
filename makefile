CC=gcc

SRCDIR=.
SHARED_OBJ=$(SRCDIR)/datacollection.c $(SRCDIR)/controller.c $(SRCDIR)/util.c

CFLAGS=-std=c11 -Wall -Wpedantic

.PHONY = clean

all: client1 client2

clean:
	rm -f client1 client2

client1: clean
	gcc $(CFLAGS) -o client1 $(SRCDIR)/client1.c $(SHARED_OBJ)

client2: clean
	gcc $(CFLAGS) -o client2 $(SRCDIR)/client2.c $(SHARED_OBJ)
