#ifndef BINARIZE_H
#define BINARIZE_H
#include <SDL2/SDL.h>
#include <err.h>
#include <gsl/gsl_matrix.h>
#include "utility.h"
#include "blurs.h"
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

/*
    Colored thresholding with automatic threshold computation
    via Otsu's method
    <surface>: the surface to be processed
    <color>: the first RGB color (when > threshold)
    <color2>: the second RGB color (when < threshold)
*/
void otsu_binarization(SDL_Surface *surface, Uint8 *color, Uint8 *color2);

/*
    Colored thresholding with automatic threshold computation
    via a Gaussian kernel of size x
    <surface>: the surface to be processed
    <x>: the kernel size
    <color>: the first RGB color (when > threshold)
    <color2>: the second RGB color (when < threshold)
*/
void gaussian_binarization(SDL_Surface *surface, int x, int c, Uint8 *color,
Uint8 *color2);

#endif