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

int clamp(int x, int min, int max)
{
    if(x < min)
        return min;
    else if(x > max)
        return max;
    else
        return x;
}