#include "noise.h"

/*
    Not callable from outside this file
    Returns a random cardinal vector
    based on the input value in [0, 7]
*/
char double_to_vect(char value)
{
    char res[2];

    switch value 
    {
        case 0:
            res = {0, 1};
            break;
        case 1:
            res = {1, 0};
            break;
        case 2:
            res = {1, 1};
            break;
        case 3:
            res = {0, -1};
            break;
        case 4:
            res = {-1, 0}
            break;
        case 5:
            res = {-1, -1};
            break;
        case 6:
            res = {1, -1};
            break;
        case 7:
            res = {-1, 1};
            break;
        default:
            warnx("double_to_vect recieved a value higher than 7 or negative");
    }

    return res;
}

gsl_matrix *perlin_matrix(size_t size)
{
    gsl_matrix *mat = gsl_matrix_calloc(size, size);
    
    for (size_t i = 0; i < mat -> size1; i++)
    {
        for (size_t j = 0; j < mat -> size2; j++)
        {
            char rand_val = rand() % 8;
            gsl_matrix_set(mat, i, j, rand_val);
        }
    }

    return mat;
}

double perlin(double x, double y)
{
    size_t X = (size_t)floor(x) & 255;
    size_t Y = (size_t)floor(y) & 255;

    x -= floor(x);
    y -= floor(y);

    double u = fade(x);
    double v = fade(y);
}