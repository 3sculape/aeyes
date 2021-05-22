#ifndef BINARIZE_H
#define BINARIZE_H
#include <SDL2/SDL.h>

/*
    Colored thresholding of the image. Each pixel strictly above
    the threshold is set to {ra, ga, ba}, and the rest
    is set to {rb, gb, bb}
    <surface>: the surface to be processed
    <threshold>: the threshold for binarization
    {ra, ga, ba}: the first RGB color (when > threshold)
    {rb, gb, bb}: the first RGB color (when < threshold)
*/
void binarization(SDL_Surface *surface, Uint32 threshold, int ra, int ga,
int ba, int rb, int gb, int bb);

/*
    Colorization of the image. Each pixel is converted to
    the color defined by the triplet {ra, ga, ba}. If
    <keep_luminance> is set, each pixel will keep its
    original L value.
    <surface>: the surface to be processed
    <threshold>: the threshold for binarization
    {ra, ga, ba}: a standard RGB color
    <keep_luminance>: a boolean
*/
void colorize(SDL_Surface *surface, int ra, int ga, int ba, int keep_luminance);

#endif