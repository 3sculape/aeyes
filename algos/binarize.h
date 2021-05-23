#ifndef BINARIZE_H
#define BINARIZE_H
#include <SDL2/SDL.h>
#include <err.h>
#include <gsl/gsl_matrix.h>
#include "../utils/lib_sdl.h"

/*
    Colored thresholding of the image. Each pixel strictly above
    the threshold is set to {ra, ga, ba}, and the rest
    is set to {rb, gb, bb}
    <surface>: the surface to be processed
    <threshold>: the threshold for binarization
    {ra, ga, ba}: the first RGB color (when > <threshold>)
    {rb, gb, bb}: the first RGB color (when < <threshold>)
*/
void binarization(SDL_Surface *surface, Uint8 threshold, int ra, int ga,
int ba, int rb, int gb, int bb);

#endif