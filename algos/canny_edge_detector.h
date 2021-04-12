#ifndef CANNY_EDGE_DETECTOR_H
#define CANNY_EDGE_DETECTOR_H

#include <gsl/gsl_matrix.h>
#include "blurs.h"
#include <stdio.h>
#include <errx.h>

SDL_Surface* canny(SDL_Surface *surface);

#endif
