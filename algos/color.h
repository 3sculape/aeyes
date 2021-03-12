#ifndef COLOR_H
#define COLOR_H
#include <SDL2/SDL.h>
#include "../utils/lib_sdl.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <err.h>
#include "utility.h"

/*
    Find the brightest pixel by iterating through
    the average RGB values and finding the highest
    <surface>: the surface in which to look for
    <*reference>: an array with the acquired RGB
    color
*/
void brightest_pixel(SDL_Surface *surface, Uint8 *reference);

#endif