#ifndef BLURS_H
#define BLURS_H

#include <SDL2/SDL.h>

//Blur the SDL_Surface *surface with the box blur.
void box_blur(SDL_Surface *surface);

//Blur the SDL_Surface *surface with the gaussian blur.
void gaussian_blur(SDL_Surface *surface, int x);
#endif /* BLURS_H */
