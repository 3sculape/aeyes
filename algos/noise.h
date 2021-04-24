#ifndef NOISE_H
#define NOISE_H
#include <err.h>
#include <SDL2/SDL.h>
#include <gsl/gsl_randist.h>
#include <stdlib.h>
#include "../utils/lib_sdl.h"
#include "convert.h"
#include "utility.h"

/*
    Allocate and return a Perlin matrix of size
    <size>. The matrix is filled with cardinal
    vectors (i.e. {0, 1}, {1, 1}, ...) represented
    by values in the range [0, 7]
    <size>: the desired size of the matrix
*/
gsl_matrix *perlin_matrix(size_t size);

/*
    Get the Perlin noise at point <x>, <y> for
    the Perlin matrix <mat>
*/
double perlin(gsl_matrix *mat, double x, double y);

#endif