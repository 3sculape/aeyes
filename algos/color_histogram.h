#ifndef COLOR_HISTOGRAM_H
#define COLOR_HISTOGRAM_H

#include "../utils/lib_sdl.h"
#include "basic.h"
#include "convert.h"

/* (DEPRECATED) Creates an image from the arrays given by color_histogram,
 * output the surface resulting from the function (Planning on using a wrapper
 * in the end)
 */
SDL_Surface* show_histogram(SDL_Surface* surface);

/* Creates the histogram in function of the luminance
 */
void clrl_histo(SDL_Surface *surface, int red[], int green[], int blue[]);

// sub function for main l histo function
void l_histo(SDL_Surface *surface, int lum[]);

// Main luminance histogram function
void show_l_histo(SDL_Surface *surface);

// Main histogram fnc, color in function of luminance (WIP)
void show_clrl_histo(SDL_Surface * surface);

/* (DEPRECATED) Main histogram function, takes the surface as parameter as
 * well as three arrays corresponding to the average of rgb in an image
 * (arrays are considered initialized by the programmer before the function
 * call)
 */
void color_histogram(SDL_Surface* surface, size_t red[], size_t green[],
        size_t blue[]);

void histo_color(SDL_Surface *surface);
#endif
