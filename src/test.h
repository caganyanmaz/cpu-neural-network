#pragma once
#include <stdlib.h>
#include "network.h"
#include "../extern/MNIST-idx1-and-idx3-file-readers/idx.h"

typedef struct
{
	Network *network;
} Test;

Test *t_create(size_t nrows, size_t ncols);
Test *t_create_with_file(size_t nrows, size_t ncols, FILE *file);
void t_destroy(Test *obj);

void t_train_for_single_epoch(Test *obj, IDX3_DATA *images, IDX1_DATA *labels, size_t l, size_t r);
void t_test_accuracy(const Test *obj, IDX3_DATA *images, IDX1_DATA *labels, size_t l, size_t r);

