#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "network.h"

#define INV_LN_2 1.44269504089

Network *n_create(double (*activate) (double), double (*d_activate) (double), size_t arg_count, ...)
{
	va_list args;
	va_start(args, arg_count);
	Network *obj = malloc(sizeof(*obj));
	obj->activate = activate;
	obj->d_activate = d_activate;
	obj->layer_count = arg_count - 1;
	obj->layers = malloc(sizeof(Layer*) * obj->layer_count);
	obj->inv_temperature = 1;
	size_t current_size = va_arg(args, size_t);
	for (int i = 0; i < obj->layer_count; i++)
	{
		size_t next_size = va_arg(args, size_t);
		(obj->layers)[i] = lyr_create(current_size, next_size);
		current_size = next_size;
	}
	va_end(args);
	return obj;
}

void destroy_layers(Network *obj)
{
	for (int i = 0; i < obj->layer_count; i++)
		lyr_destroy((obj->layers)[i]);
	free(obj->layers);
}

void n_destroy(Network *obj)
{
	destroy_layers(obj);
	free(obj);
}

double get_C_for_softmax(const Matrix *output, double inv_temperature)
{
	double mx = 0;
	for (int i = 0; i < output->height; i++)
	{
		if (m_get(output, i, 0) * inv_temperature > mx)
			mx = m_get(output, i, 0) * inv_temperature;
	}
	return -mx;
}

double get_S_for_softmax(const Matrix *output, double inv_temperature, double C)
{
	double S = 0;
	for (int i = 0; i < output->height; i++)
	{
		S += exp(m_get(output, i, 0) * inv_temperature + C);
	}
	return S;
}

void inline_softmax(Matrix *m, double inv_temperature)
{
	// x_i' = exp(x_i / T) / S
	double C = get_C_for_softmax(m, inv_temperature);
	double S = get_S_for_softmax(m, inv_temperature, C);
	double inv_S = 1 / S;
	for (int i = 0; i < m->height; i++)
	{
		m_set(m, i, 0, exp(m_get(m, i, 0) * inv_temperature + C) * inv_S);
	}
}

Matrix *n_calculate_output_d(const Network *obj, const Matrix *output, const Matrix *answer)
{
	assert(output->width == 1 && answer->width == 1 && output->height == answer->height);
	// NOTE: Answer is assumed to be a probability distribution, output is not (to save time)
	// Applying derivative formula: d_i = (1 / (ln(2) * T)) * (e^(x_i / T) / S - a_i)
	// Where S = sum of e^(x_i / T) for all i
	// Note adding C to every exponent to not make values explode
	double C = get_C_for_softmax(output, obj->inv_temperature);
	double S = get_S_for_softmax(output, obj->inv_temperature, C);
	// Calculating S
	
	double inv_S = 1 / S;
	Matrix *d = m_create(output->height, output->width);
	for (int i = 0; i < output->height; i++)
	{	
		double x_i = m_get(output, i, 0);
		double a_i = m_get(answer, i, 0);
		double d_i = INV_LN_2 * (exp(x_i * obj->inv_temperature + C) * inv_S - a_i);
		m_set(d, i, 0, d_i);
	}
	return d;
}

double n_get_cross_entropy_loss(const Network *obj, const Matrix *output, const Matrix *answer)
{
	assert(output->width == 1 && answer->width == 1 && output->height == answer->height);
	double C = get_C_for_softmax(output, obj->inv_temperature);
	double S = get_S_for_softmax(output, obj->inv_temperature, C);
	double dot_product = 0;
	for (int i = 0; i < output->height; i++)
	{
		dot_product += m_get(output, i, 0) * m_get(answer, i, 0);
	}
	return INV_LN_2 * ((log(S) - C) - obj->inv_temperature * dot_product);
}

double n_accumulate_gradient(Network *obj, Matrix *input, const Matrix *answer)
{
	assert(input->height == (obj->layers)[0]->ninodes && input->width == 1);
	// Storing the sum values
	Matrix **v_values = malloc(sizeof(Matrix*) * (obj->layer_count + 1));
	// Forward propogation
	v_values[0] = input;
	for (int i = 0; i < obj->layer_count; i++)
	{
		v_values[i+1] = lyr_forward_propogate((obj->layers)[i], v_values[i], obj->activate);
	}
	double loss = n_get_cross_entropy_loss(obj, v_values[obj->layer_count], answer);


	// Back propogation
	Matrix *current_d = n_calculate_output_d(obj, v_values[obj->layer_count], answer);
	for (int i = obj->layer_count-1; i >= 0; i--)
	{
		Matrix *next_d = lyr_back_propogate((obj->layers)[i], v_values[i], current_d, obj->activate, obj->d_activate);
		m_destroy(current_d);
		current_d = next_d;
	}
	for (int i = 1; i <= obj->layer_count; i++)
	{
		m_destroy(v_values[i]);
	}
	free(v_values);
	return loss;
}

Matrix *n_get_result(const Network *obj, const Matrix *input)
{
	assert(input->height == (obj->layers)[0]->ninodes && input->width == 1);
	Matrix *last_v = lyr_forward_propogate((obj->layers)[0], input, obj->activate);
	for (int i = 1; i < obj->layer_count; i++)
	{
		Matrix *current_v = lyr_forward_propogate((obj->layers)[i], last_v, obj->activate);
		m_destroy(last_v);
		last_v = current_v;
	}
	inline_softmax(last_v, obj->inv_temperature);
	return last_v;
}

void n_update(Network *obj, double step_size)
{
	for (int i = 0; i < obj->layer_count; i++)
	{
		lyr_update((obj->layers)[i], step_size);
	}
}

// Not saving the activation rn, it'll use leaky_ReLU
void n_save_to_file(const Network *obj, FILE *file)
{
	fprintf(file, "%lu\n", obj->layer_count);
	for (int i = 0; i < obj->layer_count; i++)
	{
		lyr_save_to_file((obj->layers)[i], file);
	}
}

Network *n_load_from_file(double (*activate) (double), double (*d_activate) (double), FILE *file)
{
	Network *obj = malloc(sizeof(*obj));
	obj->activate = activate;
	obj->d_activate = d_activate;
	fscanf(file, "%lu", &obj->layer_count);
	obj->layers = malloc(sizeof(Layer*) * obj->layer_count);
	obj->inv_temperature = 1;
	size_t last_size = 0;
	for (int i = 0; i < obj->layer_count; i++)
	{
		(obj->layers)[i] = lyr_load_from_file(file);
	}
	return obj;
}

void n_print(const Network *obj)
{
	for (int i = 0; i < obj->layer_count; i++)
	{
		printf("Layer %d:\n", i);
		lyr_print((obj->layers)[i]);
	}
}


