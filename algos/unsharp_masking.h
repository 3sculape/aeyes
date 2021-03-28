#ifndef UNSHARP_MASKING_H
#define UNSHARP_MASKING_H

#include <gsl/gsl_matrix.h>

SDL_Surface* sharpen(SDL_Surface *surface, double force, int threshold);

#endif
