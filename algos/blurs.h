#ifndef BLURS_H
#define BLURS_H

#include <SDL2/SDL.h>

// Blur the SDL_Surface *surface with the box blur.
// x parameter is the size of the kernel
void box_blur(SDL_Surface *surface, int x);

// Blur the SDL_Surface *surface with the median blur.
// x parameter is the size of the kernel
void median_blur(SDL_Surface *surface, int x);

// Blur the SDL_Surface *surface with the median blur.
// x parameter is the size of the kernel
void min_blur(SDL_Surface *surface, int x);

// Blur the SDL_Surface *surface with the median blur.
// x parameter is the size of the kernel
void max_blur(SDL_Surface *surface, int x);

// Blur the SDL_Surface *surface with the gaussian blur.
// x parameter is the size of the kernel.
void gaussian_blur(SDL_Surface *surface, int x);

// Blur the SDL_Surface *surface with the gaussian blur but faster.
// x parameter is the size of the kernel.
void fast_gaussian_blur(SDL_Surface *surface, int x);

// Blur the SDL_Surface *surface with the motion blur.
// x parameter is the length of the kernel
void motion_blur(SDL_Surface *surface, int x, double angle);

// Blur the SDL_Surface *surface with the radial blur.
// x parameter is the length of the kernel
void radial_blur(SDL_Surface *surface, int x);
#endif /* BLURS_H */
