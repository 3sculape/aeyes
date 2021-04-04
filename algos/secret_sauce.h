#ifndef SECRET_SAUCE_H
#define SECRET_SAUCE_H
#include <SDL2/SDL.h>
#include "../utils/lib_sdl.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <err.h>
#include "utility.h"

/*
    Call this function with the array {200, 60, 20}
*/
void deep_fry(SDL_Surface *surface, Uint8 *reference);

/*
    Call this function with force < 0.5 or force > 1 
*/
SDL_Surface* fever_dream(SDL_Surface *surface, double force);

#endif