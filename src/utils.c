#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "utils.h"

const char *LIGHT_SCALE = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
#define LIGHT_SCALE_SIZE 92
#define LIGHT_SCALE_FRAC LIGHT_SCALE_SIZE / (UCHAR_MAX + 1)

void shuffle(unsigned char *arr, size_t len, size_t count)
{
	char *tmp = malloc(len);
	for (int i = 1; i < count; i++)
	{
		int j = rand() % (i + 1);
		memcpy(tmp, &arr[i * len], len);
		memcpy(&arr[i * len], &arr[j * len], len);
		memcpy(&arr[j * len], tmp, len);
	}
	free(tmp);
}

void print_byte_grid(unsigned char *arr, size_t nrows, size_t ncols)
{
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			int idx = arr[i * ncols + j] * LIGHT_SCALE_FRAC;
			if (!(0 <= idx && idx < LIGHT_SCALE_SIZE))
			{
				printf("%d\n", idx);
			}
			assert(0 <= idx && idx < LIGHT_SCALE_SIZE);
			putchar(LIGHT_SCALE[idx]);
		}
		putchar('\n');
	}
}
