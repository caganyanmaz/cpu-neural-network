#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "layer.h"
#define LAYER_COUNT 4

typedef struct
{
	size_t layer_count;
	double (*activate) (double);
	double (*d_activate) (double);
	double inv_temperature;
	Layer **layers; } Network;


Network *n_create(double (*activate) (double), double (*d_activate) (double), size_t layer_count, ...);
Network *n_load_from_file(double (*activate) (double), double (*d_activate) (double), FILE *file);
void n_destroy(Network *obj);


// Returns cross-entropy loss
double n_accumulate_gradient(Network *obj, Matrix *input, const Matrix *answer);
Matrix *n_get_result(const Network *obj, const Matrix *input);
void n_update(Network *obj, double step_size);

void n_save_to_file(const Network *obj, FILE *file);

void n_print(const Network *obj);

