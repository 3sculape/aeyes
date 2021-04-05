#ifndef UNSHARP_MASKING_H
#define UNSHARP_MASKING_H

#include <gsl/gsl_matrix.h>

// Main sharpening function, returns a surface, takes force[0,1] and threshold
SDL_Surface* sharpen(SDL_Surface *surface, double force, int threshold);

#endif
