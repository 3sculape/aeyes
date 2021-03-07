#ifndef COLOR_H
#define COLOR_H
#include <SDL2/SDL.h>
#include "../Header/lib_sdl.h"

/*
    Find the brightest pixel by iterating through
    the average RGB values and finding the highest
    <surface>: the surface in which to look for
    <*coordinates>: an array with the newly computed
    coordinates
*/
void brightest_pixel(SDL_Surface *surface, int *coordinates);

#endif