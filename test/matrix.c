#include <stdlib.h>
#include <check.h>
#include "../src/matrix.h" 
#include "matrix.h"

#define MAX_MATRIX_SIZE 10
#define GENERATED_RANDOM_TEST_COUNT 1000

#define EPSILON 0.001

double rand_double()
{
	return ((double) rand()) / ((double) RAND_MAX);
}

Matrix *generate_random_matrix(int h, int w)
{
	int height = h == -1 ? 1 + rand() % (MAX_MATRIX_SIZE-1) : h;
	int width  = w == -1 ? 1 + rand() % (MAX_MATRIX_SIZE-1) : w;
	Matrix *a = m_create(height, width);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			m_set(a, i, j, rand_double());
		}
	}
	return a;
}

void check_matrices_equal(Matrix *a, Matrix *c)
{
	ck_assert_int_eq(a->height, c->height);
	ck_assert_int_eq(a->width, c->width);
	for (int i = 0; i < a->height; i++)
	{
		for (int j = 0; j < a->width; j++)
		{
			ck_assert_double_eq_tol(m_get(a, i, j), m_get(c, i, j), EPSILON);
		}
	}


}

START_TEST(test_matrix_create)
{

	Matrix *a = m_create(2, 3);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++) m_set(a, i, j, 1 + i * 3 + j);
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
			ck_assert_double_eq_tol(m_get(a, i, j), 1 + i * 3 + j, EPSILON);
	}
	m_destroy(a);
}
END_TEST

START_TEST(test_matrix_get_set)
{
	for (int l = 0; l < 10; l++)
	{
		Matrix *a = generate_random_matrix(-1, -1);
		for (int r = 0; r < GENERATED_RANDOM_TEST_COUNT; r++)
		{
			int i = rand() % a->height;
			int j = rand() % a->width;
			double val = rand_double();
			m_set(a, i, j, val);
			ck_assert_double_eq_tol(m_get(a, i, j), val, EPSILON);
		}
		m_destroy(a);
	}
}
END_TEST

START_TEST(test_matrix_mul)
{
	Matrix *a = m_create(2, 3);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++) m_set(a, i, j, 1 + i * 3 + j);
	}
	Matrix *b = m_create(3, 2);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_set(b, i, j, (i+1) * 10 + j);
		}
	}
	Matrix *c = m_mul(a, b);
	ck_assert_double_eq_tol(m_get(c, 0, 0), 140, EPSILON);
	ck_assert_double_eq_tol(m_get(c, 0, 1), 146, EPSILON);
	ck_assert_double_eq_tol(m_get(c, 1, 0), 320, EPSILON);
	ck_assert_double_eq_tol(m_get(c, 1, 1), 335, EPSILON);
	Matrix *d = m_mul(b, a);
	ck_assert_double_eq_tol(m_get(d, 0, 0), 54, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 0, 1), 75, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 0, 2), 96, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 1, 0), 104, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 1, 1), 145, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 1, 2), 186, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 2, 0), 154, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 2, 1), 215, EPSILON);
	ck_assert_double_eq_tol(m_get(d, 2, 2), 276, EPSILON);
	m_destroy(a);
	m_destroy(b);
	m_destroy(c);
	m_destroy(d);
}
END_TEST

START_TEST(test_matrix_add)
{
	Matrix *a = m_create(5, 3);
	Matrix *b = m_create(5, 3);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_set(a, i, j, 1000 * i + 100 * j);
			m_set(b, i, j, 10 * i + j);
		}
	}
	Matrix *c = m_add(a, b);
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
			ck_assert_double_eq_tol(m_get(c, i, j), 1010 * i + 101 * j, EPSILON);
	m_destroy(a);
	m_destroy(b);
	m_destroy(c);
}
END_TEST

START_TEST(test_matrix_transpose)
{
	for (int i = 0; i < GENERATED_RANDOM_TEST_COUNT; i++)
	{
		Matrix *a = generate_random_matrix(-1, -1);
		Matrix *b = m_transpose(a);
		ck_assert_int_eq(a->height, b->width);
		ck_assert_int_eq(a->width, b->height);
		for (int i = 0; i < a->height; i++)
			for (int j = 0; j < a->width; j++)
				ck_assert_double_eq_tol(m_get(a, i, j), m_get(b, j, i), EPSILON);
		m_destroy(a);
		m_destroy(b);
	}
}
END_TEST

START_TEST(test_matrix_inline_add)
{
	for (int i = 0; i < GENERATED_RANDOM_TEST_COUNT; i++)
	{
		Matrix *a = generate_random_matrix(-1, -1);
		Matrix *b = generate_random_matrix(a->height, a->width);
		Matrix *c = m_add(a, b);
		m_inline_add(a, b);
		check_matrices_equal(a, c);
		m_destroy(a);
		m_destroy(b);
		m_destroy(c);
	}
}
END_TEST

START_TEST(test_matrix_inline_mul)
{
	for (int i = 0; i < GENERATED_RANDOM_TEST_COUNT; i++)
	{
		Matrix *a = generate_random_matrix(-1, -1);
		Matrix *b = generate_random_matrix(a->width, -1);
		Matrix *c = m_mul(a, b);
		m_inline_mul(a, b);
		check_matrices_equal(a, c);
		m_destroy(a);
		m_destroy(b);
		m_destroy(c);
	}
}
END_TEST

START_TEST(test_matrix_inline_transpose)
{
	for (int i = 0; i < GENERATED_RANDOM_TEST_COUNT; i++)
	{
		Matrix *a = generate_random_matrix(-1, -1);
		Matrix *b = m_transpose(a);
		m_inline_transpose(a);
		check_matrices_equal(a, b);
		m_destroy(a);
		m_destroy(b);
	}
}
END_TEST


Suite *matrix_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Matrix");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_matrix_create);
	tcase_add_test(tc_core, test_matrix_get_set);
	tcase_add_test(tc_core, test_matrix_mul);
	tcase_add_test(tc_core, test_matrix_add);
	tcase_add_test(tc_core, test_matrix_transpose);
	tcase_add_test(tc_core, test_matrix_inline_mul);
	tcase_add_test(tc_core, test_matrix_inline_add);
	tcase_add_test(tc_core, test_matrix_inline_transpose);

	suite_add_tcase(s, tc_core);

	return s;
}
