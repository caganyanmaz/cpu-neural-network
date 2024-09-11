#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

typedef struct 
{
	size_t ninodes;
	size_t nonodes;
	Matrix *weights;
	Matrix *biases;
	Matrix *accumulated_weight_gradient;
	Matrix *accumulated_bias_gradient;
}Layer;


Layer *lyr_create(size_t ninodes, size_t nonodes);
Layer *lyr_load_from_file(FILE *file);
void lyr_destroy(Layer *obj);


Matrix *lyr_forward_propogate(const Layer *obj, const Matrix *v, double (*act) (double));
Matrix *lyr_back_propogate(Layer *obj, const Matrix *last_v, const Matrix *d, double (*act) (double), double (*dact) (double)); // d -> partial derivative of output nodes' sums (the given input before activation function), returns the same thing for the previous layer
void lyr_update(Layer *obj, double step_size);

void lyr_save_to_file(const Layer *obj, FILE *file);

void lyr_print(const Layer *obj);
