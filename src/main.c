#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../extern/MNIST-idx1-and-idx3-file-readers/idx.h"
#include "utils.h"
#include "network.h"
#include "test.h"
#include "matrix.h"

#define TRAIN_LABELS_PATH "../data/train-labels"
#define TRAIN_IMAGES_PATH "../data/train-images"
#define TEST_LABELS_PATH "../data/test-labels"
#define TEST_IMAGES_PATH "../data/test-images"
#define SEED 42 // Don't change (validation set shuffling made from that)
#define VALIDATION_RATIO 0.2


int main()
{
	srand(SEED);
	IDX1_DATA test_labels, train_labels;
	IDX3_DATA test_images, train_images;
	assert(fread_idx1_file(TEST_LABELS_PATH, &test_labels));
	assert(fread_idx1_file(TRAIN_LABELS_PATH, &train_labels));
	assert(fread_idx3_file(TEST_IMAGES_PATH, &test_images));
	assert(fread_idx3_file(TRAIN_IMAGES_PATH, &train_images));

	int train_count = train_images.nimages * VALIDATION_RATIO;
	int validation_count = train_images.nimages - train_count;

	printf("Training: %d validation: %d\n", train_count, validation_count);
	//shuffle(train_images.data, train_images.nrows * train_images.ncols, train_images.nimages);
	for (int i = 0; i < 10; i++)
	{
		int image = rand() % train_images.nimages;
		printf("%d / %d train_images.nimages\n", image, train_images.nimages);
		print_byte_grid(&train_images.data[image * train_images.nrows * train_images.ncols], train_images.nrows, train_images.ncols);
	}
	Test *test = t_create(train_images.nrows, train_images.ncols);
	n_print(test->network);
	int batch_size = train_images.nimages / 64;
	for (int i = 0;; i++)
	{
		printf("Epoch: %d\n", i+1);
		t_train_for_single_epoch(test, &train_images, &train_labels, 0, batch_size);
		if ((i % 100) == 0)
			t_test_accuracy(test, &test_images, &test_labels, 0, test_images.nimages);
	}
	n_print(test->network);
}
