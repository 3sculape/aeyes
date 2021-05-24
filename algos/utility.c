#include "utility.h"

void print_matrix(gsl_matrix *m)
{
    for (size_t i = 0; i < m -> size1; i++)
    {
        for (size_t j = 0; j < m -> size2; j++)
            printf("%f ", gsl_matrix_get(m, i, j));
        printf("\n");
    }
}

void print_vector(gsl_vector *v)
{
    for (size_t i = 0; i < v -> size; i++)
        printf("%f ", gsl_vector_get(v, i));
    printf("\n");
}

double clamp(double x, double min, double max)
{
    if(x < min)
        return min;
    else if(x > max)
        return max;
    else
        return x;
}

double distance(int ax, int ay, int bx, int by)
{
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

double lerp(double a, double b, double w)
{
    return w * a + (1 - w) * b;
}

double blerp(double a, double b, double c, double d, double tx, double ty)
{
	double x = lerp(lerp(a, b, tx), lerp(c, d, tx), ty);
	return x;
}

double fade(double t) 
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}
