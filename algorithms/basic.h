#ifndef BASIC_H
#define BASIC_H
#include <SDL2/SDL.h>

/*
    Computes the grayscale by iterating over each pixel
    and applying a formula based on its RGB value
    surface: The surface to be processed
*/
void grayscale(SDL_Surface *surface);

#endif