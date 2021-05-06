#ifndef VIGNETTE_H
#define VIGNETTE_H
#include <err.h>
#include <SDL2/SDL.h>
#include <gsl/gsl_randist.h>
#include "../utils/lib_sdl.h"
#include "convert.h"
#include "utility.h"

/*
    Apply a vignette effect with a Gaussian distribution
    towards the specified color
    <surface>: the surface to be processed
    <color>: a 3-tuple of values representing a color
    for the vignette effect.
    <factor>: the desired strength of the effect
    As always, the size of the arrays cannot be checked,
    so be wary of putting the right number of elements...
*/
void vignette(SDL_Surface *surface, Uint8 *color, double factor);

#endif