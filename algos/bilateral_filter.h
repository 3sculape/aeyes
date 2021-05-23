#ifndef BILATERAL_FILTER_H
#define BILATERAL_FILTER_H

#include <SDL2/SDL.h>
#include <gsl/gsl_math.h>
#include <math.h>
#include "../utils/lib_sdl.h"
#include "basic.h"
#include "utility.h"

// Surface blur function, all parameters can be whatever but recommended
// diameter < 20 and sigmas < 50 for shorter execution time.
void bilateral_filter(SDL_Surface* original, double diameter, double sigmaI,
                      double sigmaS);

#endif //BILATERAL_FILTER_H
