all: main.c bitset.c bitset.h bloom.c bloom.h
	gcc -Wall -g -o prog bitset.c main.c bloom.c -I.

test: all
	./runtest

clean:
	rm prog
