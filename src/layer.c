#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

double clear_func(double in)
{
	return 0;
}

double rand_func(double in)
{
	return ((double)random()) / RAND_MAX;
}

#define NORMAL_DIST_ITER_COUNT 100
#define NORMAL_DIST_FACT (sqrt(12) / sqrt(NORMAL_DIST_ITER_COUNT))
double get_normal_random(double mean, double var)
{
	double sum = 0;
	for (int i = 0; i < NORMAL_DIST_ITER_COUNT; i++)
	{
		sum += ((double)random()) / RAND_MAX;
	}
	return (sum - NORMAL_DIST_ITER_COUNT * 0.5) * NORMAL_DIST_FACT * var + mean;
}

double tmpninodes;
double tmpnonodes;
double normal_rand_func(double in)
{
	return get_normal_random(0, sqrt(2.0 / (tmpninodes + tmpnonodes)));
}

void lyr_create_accumulated_gradients(Layer *obj)
{
	assert(obj->biases->width == 1);
	obj->accumulated_weight_gradient = m_create(obj->weights->height, obj->weights->width);
	obj->accumulated_bias_gradient   = m_create(obj->biases->height, 1);
	m_inline_apply(obj->accumulated_bias_gradient, clear_func);
	m_inline_apply(obj->accumulated_weight_gradient, clear_func);
}

Layer *lyr_create(size_t ninodes, size_t nonodes)
{
	Layer *obj = malloc(sizeof(*obj));
	obj->ninodes = ninodes;
	obj->nonodes = nonodes;
	obj->weights = m_create(nonodes, ninodes);
	obj->biases  = m_create(nonodes, 1);
	tmpninodes = obj->ninodes;
	tmpnonodes = obj->ninodes;
	m_inline_apply(obj->weights, normal_rand_func);
	m_inline_apply(obj->biases, clear_func);
	lyr_create_accumulated_gradients(obj);
	return obj;
}
void lyr_destroy(Layer *obj)
{
	free(obj->weights);
	free(obj->biases);
	free(obj->accumulated_bias_gradient);
	free(obj->accumulated_weight_gradient);
	free(obj);
}

Matrix *lyr_forward_propogate(const Layer *obj, const Matrix *v, double (*act) (double))
{
	assert(v->width == 1 && v->height == obj->ninodes);
	Matrix *m = m_apply(v, act);
	m_inline_rmul(m, obj->weights);
	m_inline_add(m, obj->biases);
	return m;
}


// d is the partial derivative of sums with respect to the loss function
// last_v is the last sums (values)
Matrix *lyr_back_propogate(Layer *obj, const Matrix *last_v, const Matrix *d, double (*act) (double), double (*dact) (double))
{
	assert(d->width == 1 && d->height == obj->nonodes);
	assert(last_v->width == 1 && last_v->height == obj->ninodes);

	// Calculating the derivatives of biases
	m_inline_add(obj->accumulated_bias_gradient, d);

	// Calculating the derivatives of weights
	Matrix *x_t = m_apply(last_v, act);
	m_inline_transpose(x_t);
	Matrix *weights_gradient_change = m_mul(d, x_t);
	m_inline_add(obj->accumulated_weight_gradient, weights_gradient_change);
	
	// Calculating the next derivatives
	Matrix *dx = m_transpose(d);
	m_inline_mul(dx, obj->weights);
	m_inline_transpose(dx);
	Matrix *prev_d = m_apply(last_v, dact);
	m_inline_point_mul(prev_d, dx);
	m_destroy(x_t);
	m_destroy(weights_gradient_change);
	m_destroy(dx);
	return prev_d;
}

void lyr_update(Layer *obj, double step_size)
{
	// Updating the weights
	m_inline_scalar_mul(obj->accumulated_weight_gradient, -step_size);
	m_inline_add(obj->weights, obj->accumulated_weight_gradient);
	m_inline_apply(obj->accumulated_weight_gradient, clear_func);

	// Updating the biases
	m_inline_scalar_mul(obj->accumulated_bias_gradient, -step_size);
	m_inline_add(obj->biases, obj->accumulated_bias_gradient);
	m_inline_apply(obj->accumulated_bias_gradient, clear_func);
}

void lyr_save_to_file(const Layer *obj, FILE *file)
{
	fprintf(file, "%lu %lu\n", obj->ninodes, obj->nonodes);
	m_save_to_file(obj->weights, file);
	m_save_to_file(obj->biases, file);
}

Layer *lyr_load_from_file(FILE *file)
{
	Layer *obj = malloc(sizeof(*obj));
	fscanf(file, "%lu %lu", &obj->ninodes, &obj->nonodes);
	obj->weights = m_load_from_file(file);
	obj->biases  = m_load_from_file(file);
	lyr_create_accumulated_gradients(obj);
	return obj;
}

void lyr_print(const Layer *obj)
{
	printf("Biases:\n");
	m_print(obj->biases);
	printf("Weights:\n");
	m_print(obj->weights);
}
