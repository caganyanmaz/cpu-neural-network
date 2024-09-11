#include "algorithms.h"
#include "matrix.h"
#include "network.h"

#define IMAGE_HEIGHT 28
#define IMAGE_WIDTH 28
#define DIGIT_COUNT 10


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

int api_get_result(Network *network, unsigned char values[IMAGE_HEIGHT][IMAGE_WIDTH])
{
	Matrix *input = m_create(IMAGE_HEIGHT, IMAGE_WIDTH);
	for (int i = 0; i < IMAGE_HEIGHT; i++)
	{
		for (int j = 0; j < IMAGE_WIDTH; j++)
		{
			m_set(input, i * IMAGE_WIDTH + j, 0, values[i][j]);
		}
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

