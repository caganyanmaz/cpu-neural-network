#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include "algorithms.h"
#include "test.h"

#define DIGIT_COUNT 10 // In case you don't know
#define GRADIENT_CHANGE_EPSILON 1e-5


Test *t_create(size_t nrows, size_t ncols)
{
	Test *obj = malloc(sizeof(*obj));
	obj->network = n_create(leaky_ReLU, d_leaky_ReLU, 4, (int)nrows * ncols, 16, 16, DIGIT_COUNT);
	return obj;
}

Test *t_create_with_file(size_t nrows, size_t ncols, FILE *file)
{
	Test *obj = malloc(sizeof(*obj));
	obj->network = n_load_from_file(leaky_ReLU, d_leaky_ReLU, file);
	return obj;
}

void t_destroy(Test *obj)
{
	n_destroy(obj->network);
	free(obj);
}

void t_create_input(Matrix *input, const IDX3_DATA *images, size_t image_size, size_t i)
{ for (int j = i * image_size; j < (i+1) * image_size; j++)
	{
		m_set(input, j - i * image_size, 0, (double)((images->data)[j]) / ((double)UCHAR_MAX));
	}
}

void t_create_answer(Matrix *answer, IDX1_DATA *labels, size_t i)
{
	int label_digit = (labels->data)[i];
	assert(0 <= label_digit && label_digit < DIGIT_COUNT);
	for (int i = 0; i < DIGIT_COUNT; i++)
	{
		m_set(answer, i, 0, i == label_digit ? 1.0 : 0.0);
	}
}

void t_train_for_single_epoch(Test *obj, IDX3_DATA *images, IDX1_DATA *labels, size_t l, size_t r)
{
	size_t image_size = images->nrows * images->ncols;
	assert(image_size == (obj->network->layers)[0]->ninodes);
	Matrix *input = m_create(image_size, 1);
	Matrix *answer = m_create(DIGIT_COUNT, 1);
	double total_loss = 0;
	for (int i = l; i < r; i++)
	{
		t_create_input(input, images, image_size, i);
		t_create_answer(answer, labels, i);
		double loss = n_accumulate_gradient(obj->network, input, answer);
		total_loss += loss;
	}
	n_update(obj->network, GRADIENT_CHANGE_EPSILON);
}

void t_test_accuracy(const Test *obj, IDX3_DATA *images, IDX1_DATA *labels, size_t l, size_t r)
{
	size_t image_size = images->nrows * images->ncols;
	assert(image_size == (obj->network->layers)[0]->ninodes);
	Matrix *input = m_create(image_size, 1);
	Matrix *answer = m_create(DIGIT_COUNT, 1);
	int ncorrect = 0;
	for (int i = l; i < r; i++)
	{
		t_create_input(input, images, image_size, i);
		t_create_answer(answer, labels, i);
		Matrix *output = n_get_result(obj->network, input);
		assert(output->height == DIGIT_COUNT);
		int res = 0;
		for (int j = 1; j < DIGIT_COUNT; j++)
		{
			if (m_get(output, j, 0) > m_get(output, res, 0))
				res = j;
		}
		if (res == (labels->data)[i])
			ncorrect++;
	}
	double accuracy = (double) ncorrect / (r - l);
	printf("Accuracy: %f\n", accuracy);
}


