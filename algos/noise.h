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
    Gives the value for Perlin noise at coordinates
    <x>, <y>.
    <x>, <y>: a pair of coordinates (can be any value)
*/
double perlin(double x, double y, char r);

/*
    Applies Perlin noise to the luminance of the image
    <surface>: the surface to be processed
*/
void noise(SDL_Surface *surface);

#endif