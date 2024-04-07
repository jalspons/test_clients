CC=gcc

SRCDIR=.
SHARED_OBJ= \
	$(SRCDIR)/controller.c \
	$(SRCDIR)/datacollection.c \
	$(SRCDIR)/state.c \
	$(SRCDIR)/util.c 

CFLAGS=-std=c11 -Wall -Wpedantic

.PHONY = clean

all: client1

clean:
	rm -f client1 client2

client1: clean
	$(CC) $(CFLAGS) -o client1 $(SRCDIR)/client1.c $(SHARED_OBJ)

client2: clean
	$(CC) $(CFLAGS) -o client2 $(SRCDIR)/client2.c $(SHARED_OBJ)
