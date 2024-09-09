CC = gcc
DEV_FLAGS = -g3
RELEASE_FLAGS = -s -O3

release: *.c
	$(CC) $(RELEASE_FLAGS) *.c -o release

matrix.o: matrix.h matrix.c
	$(CC) $(DEV_FLAGS) -c matrix.c

main.o: main.c
	$(CC) $(DEV_FLAGS) -c main.c

dev: main.o matrix.o
	$(CC) $(DEV_FLAGS) main.o matrix.o -o dev

test: dev
	./dev
	rm dev *.o

debug: dev
	gdb ./dev

clear:
	rm -f dev release main.o matrix.o
