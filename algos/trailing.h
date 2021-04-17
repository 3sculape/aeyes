#ifndef TRAILING_H
#define TRAILING_H
#include <SDL2/SDL.h>
#include "../utils/lib_sdl.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <err.h>
#include <math.h>
#include "utility.h"
#include "convert.h"
#include <time.h>
#include <stdlib.h>

void global_trailing_v(SDL_Surface *surface, int start_pixel, int inverse);

void global_trailing_h(SDL_Surface *surface, int start_pixel, int inverse);

void edge_trailing_soufflerie(SDL_Surface *surface, SDL_Surface *edge_map,
    int kernel_size, int inverse);

void edge_trailing_zigzag(SDL_Surface *surface, SDL_Surface *edge_map,
    int kernel_size);

#endif