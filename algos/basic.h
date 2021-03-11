#ifndef BASIC_H
#define BASIC_H
#include <SDL2/SDL.h>
#include <err.h>
#include "../utils/lib_sdl.h"

/*
    All algorithms provided in this header do not
    return; instead, they modify the provided SDL
    surface in-place. The pointer is thus set
    directly to the processed image which prevents
    unneeded address copying
*/

/*
    Compute the grayscale by iterating over each pixel
    and applying a formula based on its RGB value
    <surface>: the surface to be processed
*/
void grayscale(SDL_Surface *surface);

/*
    Simple thresholding of the image. Each pixel strictly above
    the threshold is set to white, and the rest to black
    <surface>: the surface to be processed
    <threshold>: the threshold for binarization
*/
void binarization(SDL_Surface *surface, char threshold);

/*
    Negative of the image. For each pixel, substract its current
    red value from 255 to obtain its new red value, and repeat for
    all colors.
    <surface>: the surface to be processed
*/
void negative(SDL_Surface *surface);

/*
    Image clipping. Turn all fully white pixels into red, and turn
    all fully black pixels into blue.
    <surface>: the surface to be processed
*/
void clipping(SDL_Surface *surface);

#endif