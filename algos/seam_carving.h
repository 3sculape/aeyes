#ifndef SEAM_CARVING_H
#define SEAM_CARVING_H
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

void power_map(SDL_Surface *surface, SDL_Surface *edge_map);

SDL_Surface* power_map_new(SDL_Surface *surface, SDL_Surface *edge_map,
 int nb_to_remove);

int min_three(int a, int b, int c);

double min_three_double(double a, double b, double c);

int min_two(int a, int b);


SDL_Surface* seam_hypot(SDL_Surface *surface);

#endif