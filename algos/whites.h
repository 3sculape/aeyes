#ifndef WHITES_H
#define WHITES_H
#include <SDL2/SDL.h>
#include <err.h>
#include "../utils/lib_sdl.h"
#include "convert.h"

void blacks(SDL_Surface *surface, double factor);

void shadows(SDL_Surface *surface, double factor);

void highlights(SDL_Surface *surface, double factor);

void whites(SDL_Surface *surface, double factor);

#endif