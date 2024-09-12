#include <limits.h>
#include <assert.h>
#include "algorithms.h"
#include "matrix.h"
#include "network.h"

#define IMAGE_HEIGHT 28
#define IMAGE_WIDTH 28
#define DIGIT_COUNT 10
#define IMAGE_SIZE (IMAGE_HEIGHT * IMAGE_WIDTH)


Network *api_load_network_from_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	return n_load_from_file(leaky_ReLU, d_leaky_ReLU, file);
	fclose(file);
}

void api_destroy_network(Network *network)
{
	n_destroy(network);
}

int api_get_result(Network *network, unsigned char values[IMAGE_SIZE])
{
	Matrix *input = m_create(IMAGE_SIZE, 1);
	for (int i = 0; i < IMAGE_SIZE; i++)
	{
		m_set(input, i, 0, ((double)values[i]) / UCHAR_MAX);
	}
	Matrix *output = n_get_result(network, input);
	int res = 0;
	for (int i = 1; i < DIGIT_COUNT; i++)
	{
		res = (m_get(output, i, 0) > m_get(output, res, 0)) ? i : res;
	}
	free(input);
	free(output);
	return res;
}

