#ifndef CANNY_EDGE_DETECTOR_H
#define CANNY_EDGE_DETECTOR_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>
#include "blurs.h"
#include <stdio.h>
#include <err.h>
#include "blurs.h"
#include "../utils/lib_sdl.h"
#include "basic.h"
#include <math.h>

void convolve_around_pixel(SDL_Surface *surface,SDL_Surface* copy
        ,int x, int y, gsl_matrix* ker);

SDL_Surface* canny_fnc(SDL_Surface *surface);

#endif
