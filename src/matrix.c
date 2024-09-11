#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 
#include <math.h>
#include "matrix.h"

Matrix *m_create(int height, int width)
{
	Matrix *obj = malloc(sizeof(*obj));
	obj->width = width;
	obj->height = height;
	obj->arr = malloc(width * height * sizeof(*(obj->arr)));
	return obj;
}

void m_destroy(Matrix *obj)
{
	free(obj->arr);
	free(obj);
}

// Moving other's values to obj
void m_move(Matrix *obj, Matrix *other)
{
	obj->width = other->width;
	obj->height = other->height;
	free(obj->arr);
	obj->arr = other->arr;
	other->arr = NULL;
	m_destroy(other);
}

Matrix *m_mul(const Matrix *a, const Matrix *b)
{
	assert(a->width == b->height);
	Matrix *res = m_create(a->height, b->width);
	for (int i = 0; i < a->height; i++)
	{
		for (int j = 0; j < b->width; j++)
		{
			double sum = 0;
			for (int k = 0; k < a->width; k++)
				sum += m_get(a, i, k) * m_get(b, k, j);
			m_set(res, i, j, sum);
		}
	}
	return res;
}

Matrix *m_add(const Matrix *a, const Matrix *b)
{
	assert(a->width == b->width && a->height == b->height);
	Matrix *res = m_create(a->height, a->width);
	for (int i = 0; i < a->height; i++)
	{
		for (int j = 0; j < a->width; j++)
		{
			m_set(res, i, j, m_get(a, i, j) + m_get(b, i, j));
		}
	}
	return res;
}

Matrix *m_transpose(const Matrix *a)
{
	Matrix *res = m_create(a->width, a->height);
	for (int i = 0; i < a->height; i++)
	{
		for (int j = 0; j < a->width; j++)
		{
			m_set(res, j, i, m_get(a, i, j));
		}
	}

	return res;
}

Matrix *m_apply(const Matrix *obj, double (*fnc) (double))
{
	Matrix *m = m_create(obj->height, obj->width);
	for (int i = 0; i < obj->height; i++)
	{
		for (int j = 0; j < obj->width; j++)
		{
			m_set(m, i, j, fnc(m_get(obj, i, j)));
		}
	}
	return m;
}

Matrix *m_point_mul(const Matrix *a, const Matrix *b)
{
	assert(a->height == b->height && a->width == b->width);
	Matrix *m = m_create(a->height, a->width);
	for (int i = 0; i < a->height; i++)
	{
		for (int j = 0; j < a->width; j++)
		{
			m_set(m, i, j, m_get(a, i, j) * m_get(b, i, j));
		}
	}
	return m;
}

Matrix *m_scalar_mul(const Matrix *a, double v)
{
	Matrix *m = m_create(a->height, a->width);
	for (int i = 0; i < a->height; i++)
	{
		for (int j = 0; j < a->width; j++)
		{
			m_set(m, i, j, m_get(a, i, j) * v);
		}
	}
	return m;
}

// TODO: Write optimized code for inline operations
// (Currently it just uses non-inline calculation than moves the result)
void m_inline_mul(Matrix *obj, const Matrix *b)
{
	Matrix *res = m_mul(obj, b);
	m_move(obj, res);
}

void m_inline_rmul(Matrix *obj, const Matrix *b)
{
	Matrix *res = m_mul(b, obj);
	m_move(obj, res);
}

void m_inline_add(Matrix *obj, const Matrix *b)
{
	Matrix *res = m_add(obj, b);
	m_move(obj, res);
}

void m_inline_transpose(Matrix *obj)
{
	Matrix *res = m_transpose(obj);
	m_move(obj, res);
}

void m_inline_apply(Matrix *obj, double (*fnc) (double))
{
	Matrix *res = m_apply(obj, fnc);
	m_move(obj, res);
}

void m_inline_point_mul(Matrix *obj, const Matrix *b)
{
	Matrix *res = m_point_mul(obj, b);
	m_move(obj, res);
}

void m_inline_scalar_mul(Matrix *obj, double v)
{
	Matrix *res = m_scalar_mul(obj, v);
	m_move(obj, res);
}

void m_set(Matrix *obj, int i, int j, double v)
{
	assert(m_is_in_range(obj, i, j));
	obj->arr[i * obj->width + j] = v;
}

double m_get(const Matrix *obj, int i, int j)
{
	return obj->arr[i * obj->width + j];
}

bool m_is_in_range(const Matrix *obj, int i, int j)
{
	return 0 <= i && i < obj->height && 0 <= j && j < obj->width;
}

double m_get_norm(const Matrix *obj)
{
	double sum = 0;
	for (int i = 0; i < obj->height; i++)
	{
		for (int j = 0; j < obj->width; j++)
		{
			sum += m_get(obj, i, j) * m_get(obj, i, j);
		}
	}
	return sqrt(sum);
}

void m_print(const Matrix *obj)
{
	for (int i = 0; i < obj->height; i++)
	{
		for (int j = 0; j < obj->width; j++)
		{
			printf("%f ", m_get(obj, i, j));
		}
		printf("\n");
	}
	printf("\n");
}


