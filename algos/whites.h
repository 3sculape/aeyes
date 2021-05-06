#ifndef WHITES_H
#define WHITES_H
#include <SDL2/SDL.h>
#include <err.h>
#include <gsl/gsl_randist.h>
#include "../utils/lib_sdl.h"
#include "convert.h"
#include "utility.h"

/*
    Changes the lightness of the darkest pixels
    in the surface according to a factor.
    Targets pixels with lightness [0, 20]
    <surface>: the surface to be processed
    <factor>: represents the effect, positive
    values will increase lightness, negative
    values will decrease it (range: [-100, 100])
*/
void blacks(SDL_Surface *surface, double factor);

/*
    Changes the lightness of darker pixels
    in the surface according to a factor.
    Targets pixels with lightness [0, 30]
    <surface>: the surface to be processed
    <factor>: represents the effect, positive
    values will increase lightness, negative
    values will decrease it (range: [-100, 100])
*/
void shadows(SDL_Surface *surface, double factor);

/*
    Changes the lightness of lighter pixels
    in the surface according to a factor.
    Targets pixels with lightness [70, 100]
    <surface>: the surface to be processed
    <factor>: represents the effect, positive
    values will increase lightness, negative
    values will decrease it (range: [-100, 100])
*/
void highlights(SDL_Surface *surface, double factor);

/*
    Changes the lightness of the lightest pixels
    in the surface according to a factor.
    Targets pixels with lightness [80, 100]
    <surface>: the surface to be processed
    <factor>: represents the effect, positive
    values will increase lightness, negative
    values will decrease it (range: [-100, 100])
*/
void whites(SDL_Surface *surface, double factor);

#endif