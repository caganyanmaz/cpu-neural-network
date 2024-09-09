CC = gcc
DEV_FLAGS = -g3
RELEASE_FLAGS = -s -O3
IDX_READER_DIR=../extern/MNIST-idx1-and-idx3-file-readers


test: dev
	mv dev ../data
	(cd ../data && ./dev)
	rm ../data/dev *.o

dev: main.o matrix.o idx.o utils.o
	$(CC) $(DEV_FLAGS) main.o matrix.o utils.o idx.o -o dev

main.o: main.c 
	$(CC) $(DEV_FLAGS) -c main.c

matrix.o: matrix.h matrix.c
	$(CC) $(DEV_FLAGS) -c matrix.c

idx.o: $(IDX_READER_DIR)/idx.c $(IDX_READER_DIR)/idx.h
	$(CC) $(DEV_FLAGS) -c $(IDX_READER_DIR)/idx.c

utils.o: utils.h utils.c
	$(CC) $(DEV_FLAGS) -c utils.c
debug: dev
	gdb ./dev

clear:
	rm -f dev release main.o matrix.o idx.o utils.o
