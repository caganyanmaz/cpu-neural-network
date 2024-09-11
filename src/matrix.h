#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
	int height;
	int width;
	double *arr;
} Matrix;

Matrix *m_create(int height, int width);
void *m_load_from_file(FILE *file);
void m_destroy(Matrix *obj);
void m_move(Matrix *obj, Matrix *other);

Matrix *m_mul(const Matrix *a, const Matrix *b);
Matrix *m_add(const Matrix *a, const Matrix *b);
Matrix *m_transpose(const Matrix *a);
Matrix *m_apply(const Matrix *obj, double (*fnc) (double));
Matrix *m_point_mul(const Matrix *a, const Matrix *b);
Matrix *m_scalar_mul(const Matrix *a, double v);

void m_inline_mul(Matrix *obj, const Matrix *b);
void m_inline_rmul(Matrix *obj, const Matrix *b);
void m_inline_add(Matrix *obj, const Matrix *b);
void m_inline_transpose(Matrix *obj);
void m_inline_apply(Matrix *obj, double (*fnc) (double));
void m_inline_point_mul(Matrix *obj, const Matrix *b);
void m_inline_scalar_mul(Matrix *obj, double v);


void m_set(Matrix *obj, int i, int j, double v);
double m_get(const Matrix *obj, int i, int j);

double m_get_norm(const Matrix *obj);

bool m_is_in_range(const Matrix *obj, int i, int j);

void m_save_to_file(const Matrix *obj, FILE *file);

void m_print(const Matrix *obj);
