CC=gcc

SRCDIR=.
SHARED_OBJ=$(SRCDIR)/datacollection.c

.PHONY = clean

all: client1

clean:
	rm -f client1

client1: clean
	gcc -o client1 $(SRCDIR)/client1.c $(SHARED_OBJ)
