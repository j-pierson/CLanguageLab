CC     = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

all: arrays pointers

arrays: memcheck.o 1dCA-Arrays.o
	$(CC) memcheck.o 1dCA-Arrays.o -o arrays

1dCA-Arrays.o: 1dCA-Arrays.c
	$(CC) $(CFLAGS) -c 1dCA-Arrays.c

pointers: memcheck.o 1dCA-Pointers.o
	$(CC) memcheck.o 1dCA-Pointers.o -o pointers

1dCA-Pointers.o: 1dCA-Pointers.c
	$(CC) $(CFLAGS) -c 1dCA-Pointers.c

memcheck.o: memcheck.c
	$(CC) $(CFLAGS) -c memcheck.c

check:
	c_style_check sorter.c

clean:
	rm -f arrays pointers *.o
