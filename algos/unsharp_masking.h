#ifndef UNSHARP_MASKING_H
#define UNSHARP_MASKING_H

#include <gsl/gsl_matrix.h>

SDL_Surface* sharpen(SDL_Surface *surface, double force, SDL_Renderer *renderer);
SDL_Surface* sharpen2(SDL_Surface *surface, double force);

#endif
