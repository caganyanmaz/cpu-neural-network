#include <stdbool.h>

typedef struct
{
	int height;
	int width;
	double *arr;
} Matrix;

Matrix *m_create(int height, int width);
void m_destroy(Matrix *obj);

Matrix *m_mul(const Matrix *a, const Matrix *b);
Matrix *m_add(const Matrix *a, const Matrix *b);
Matrix *m_transpose(const Matrix *a);

void m_inline_mul(Matrix *obj, const Matrix *b);
void m_inline_add(Matrix *obj, const Matrix *b);
void m_inline_transpose(Matrix *obj);

void m_set(Matrix *obj, int i, int j, double v);
double m_get(const Matrix *obj, int i, int j);

bool m_is_in_range(const Matrix *obj, int i, int j);

void m_print(const Matrix *obj);
