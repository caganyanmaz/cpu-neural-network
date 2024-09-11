CC = gcc
DEV_FLAGS = -g3
RELEASE_FLAGS =-s -O3
API_O_FLAGS =-s -O3 -fPIC -c
API_SO_FLAGS=-s -O3 -shared
IDX_READER_DIR=../extern/MNIST-idx1-and-idx3-file-readers
LIBRARIES=-lm


test: dev
	mv dev ../data
	(cd ../data && ./dev)
	rm ../data/dev *.o

release: *.c *.h $(IDX_READER_DIR)/idx.h $(IDX_READER_DIR)/idx.c
	$(CC) $(RELEASE_FLAGS) *.c $(IDX_READER_DIR)/idx.c $(LIBRARIES) -o release

dev: main.o matrix.o idx.o utils.o test.o layer.o network.o algorithms.o
	$(CC) $(DEV_FLAGS) main.o matrix.o utils.o idx.o test.o layer.o network.o algorithms.o $(LIBRARIES) -o dev
main.o: main.c 
	$(CC) $(DEV_FLAGS) -c main.c

matrix.o: matrix.h matrix.c
	$(CC) $(DEV_FLAGS) -c matrix.c

idx.o: $(IDX_READER_DIR)/idx.c $(IDX_READER_DIR)/idx.h
	$(CC) $(DEV_FLAGS) -c $(IDX_READER_DIR)/idx.c

utils.o: utils.h utils.c
	$(CC) $(DEV_FLAGS) -c utils.c

test.o: test.h test.c
	$(CC) $(DEV_FLAGS) -c test.c
layer.o: layer.h layer.c
	$(CC) $(DEV_FLAGS) -c layer.c

network.o: network.h network.c
	$(CC) $(DEV_FLAGS) -c network.c

algorithms.o: algorithms.h algorithms.c
	$(CC) $(DEV_FLAGS) -c algorithms.c


api: api.o api_algorithms.o api_network.o api_layer.o api_matrix.o
	$(CC) $(API_SO_FLAGS) api.o api_algorithms.o api_network.o api_layer.o api_matrix.o -o api.so

api_matrix.o: matrix.h matrix.c
	$(CC) $(API_O_FLAGS) matrix.c -o api_matrix.o

api_layer.o: layer.h layer.c
	$(CC) $(API_O_FLAGS) layer.c -o api_layer.o

api_network.o: network.h network.c
	$(CC) $(API_O_FLAGS) network.c -o api_network.o

api_algorithms.o: algorithms.h algorithms.c
	$(CC) $(API_O_FLAGS) algorithms.c -o api_algorithms.o

api.o: api.c
	$(CC) $(API_O_FLAGS) api.c




debug: dev
	gdb ./dev

clear: clear_intermediary
	rm -f dev release api.so

clear_intermediary:
	rm -f *.o
