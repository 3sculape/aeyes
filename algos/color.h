#ifndef COLOR_H
#define COLOR_H
#include <SDL2/SDL.h>
#include "../utils/lib_sdl.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <err.h>
#include <math.h>
#include "utility.h"
#include "convert.h"

/*
    Find the brightest pixel by iterating through
    the average RGB values and finding the highest
    <surface>: the surface in which to look for
    <*reference>: an array with the acquired RGB
    color
*/
void brightest_pixel(SDL_Surface *surface, Uint8 *reference);

/*
    Calibrate the image towards a blue or orange
    hue depending on a factor:
     - [-1; 0] -> towards blue
     - [0] -> no balancing
     - [0; 1] -> towards orange
    <surface>: the surface to be processed
    <factor>: a factor representing the amount of
    calibration to apply
*/
void white_balance(SDL_Surface *surface, double factor);

#endif