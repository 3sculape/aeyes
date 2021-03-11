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

/*
    Balance the whites in the surface by computing
    a matrix multiplication based on a reference
    pixel (generally considered the brightest in the
    image, although it does not have to)
    NOTE: The reference parameter from the brightest_pixel
    function can be used as a parameter to this function.
    <surface>: the surface to be processed
    <*reference>: an array with the pixel considered
    brightest in the image
*/
void white_balance(SDL_Surface *surface, Uint8 *reference);

#endif