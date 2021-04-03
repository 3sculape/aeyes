#ifndef COLOR_HISTOGRAM_H
#define COLOR_HISTOGRAM_H

#include "../utils/lib_sdl.h"
#include "basic.h"

/* Creates an image from the arrays given by color_histogram, output the
 * surface resulting from the function (Planning on using a wrapper in the end)
 */
SDL_Surface* show_histogram(SDL_Surface* surface);

/* Main histogram function, takes the surface as parameter as well as three
 * arrays corresponding to the average of rgb in an image (arrays are
 * considered initialized by the programmer before the function call)
 */
void color_histogram(SDL_Surface* surface, size_t red[], size_t green[],
        size_t blue[]);

#endif
