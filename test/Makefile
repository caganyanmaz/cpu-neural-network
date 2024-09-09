
CC=gcc
CLIBS=-lcheck -lm -lpthread -lrt -lsubunit
CFLAGS=-g3

test: final
	./final
	make clear

final: matrix.o main.o
	$(CC) $(CFLAGS) matrix.o ../src/matrix.o main.o $(CLIBS) -o final

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

matrix.o: matrix.c
	(cd ../src && $(MAKE) matrix.o)
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

clear:
	(cd ../src && $(MAKE) clear)
	rm -f final matrix.o main.o

