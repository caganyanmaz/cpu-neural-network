#include <math.h>

double ReLU(double x)
{
	return x >= 0 ? x : 0;
}

double d_ReLU(double x)
{
	return x >= 0 ? 1 : 0;
}


#define LEAK_RATE 0.1
double leaky_ReLU(double x)
{
	return x >= 0 ? x : LEAK_RATE * x;
}

double d_leaky_ReLU(double x)
{
	return x >= 0 ? 1 : LEAK_RATE;
}

#define ALPHA 1
double ELU(double x)
{
	return x >= 0 ? x : ALPHA * (exp(x) - 1);
}

double d_ELU(double x)
{
	return x >= 0 ? 1 : ALPHA * exp(x);
}
