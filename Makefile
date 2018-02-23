CC=gcc
CFLAGS=-ggdb3 -fPIC -pedantic -Wall -Werror -lpthread
DEPS=my_malloc.h

all: lib
lib: libmymalloc.so

libmymalloc.so: my_malloc.o
	$(CC) $(CFLAGS) -shared -o $@ $< -g

%.o: %.c my_malloc.h
	$(CC) $(CFLAGS) -c -o $@ $< -g

clean:
	rm -f *~ *.o *.so

clobber:
	rm -f *~ *.o
