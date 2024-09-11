#include <stdio.h>
#include <pthread.h>
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

volatile bool continue_running_main_thread = true;
const char *DEFAULT_MODEL_FILE_NAME = "default.model";

void *train_model(void *_args)
{
	// Reading training / test data
	srand(SEED);
	IDX1_DATA test_labels, train_labels;
	IDX3_DATA test_images, train_images;
	assert(fread_idx1_file(TEST_LABELS_PATH, &test_labels));
	assert(fread_idx1_file(TRAIN_LABELS_PATH, &train_labels));
	assert(fread_idx3_file(TEST_IMAGES_PATH, &test_images));
	assert(fread_idx3_file(TRAIN_IMAGES_PATH, &train_images));

	//shuffle(train_images.data, train_images.nrows * train_images.ncols, train_images.nimages);
	// Testing some random numbers
	for (int i = 0; i < 10; i++)
	{
		int image = rand() % train_images.nimages;
		printf("%d / %d train_images.nimages\n", image, train_images.nimages);
		print_byte_grid(&train_images.data[image * train_images.nrows * train_images.ncols], train_images.nrows, train_images.ncols);
	}
	// Load file if input given
	Test *test;
	char *filename = _args;
	if (filename != NULL)
	{
		FILE *file = fopen(filename, "r");
		test = t_create_with_file(train_images.nrows, train_images.ncols, file);
		fclose(file);
	}
	// Create empty with default values if there's no input
	else 	
	{
		test = t_create(train_images.nrows, train_images.ncols);
	}
	n_print(test->network);
	int batch_size = train_images.nimages / 64;
	for (int i = 0; continue_running_main_thread; i++)
	{
		printf("Epoch: %d\n", i+1);
		t_train_for_single_epoch(test, &train_images, &train_labels, 0, batch_size);
		if ((i % 100) == 0)
			t_test_accuracy(test, &test_images, &test_labels, 0, test_images.nimages);
	}

	n_print(test->network);

	{
		FILE *file = fopen(DEFAULT_MODEL_FILE_NAME, "w");
		printf("Saving model to file %s", DEFAULT_MODEL_FILE_NAME);
		n_save_to_file(test->network, file);
		fclose(file);
	}
	printf("Exiting thread...\n");
	return NULL;
}


int main(int argc, char **argv)
{
	pthread_t main_thread;
	char *filename = NULL;
	if (argc > 1)
		filename = argv[1];
	// No files
	pthread_create(&main_thread, NULL, train_model, filename);
	while (getchar() != EOF);
	continue_running_main_thread = false;
	pthread_join(main_thread, NULL);
}
