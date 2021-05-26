#ifndef NOISE_H
#define NOISE_H
#include <err.h>
#include <SDL2/SDL.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <stdlib.h>
#include "../utils/lib_sdl.h"
#include "convert.h"
#include "utility.h"

/*
    Gives the value for Perlin noise at coordinates
    <x>, <y>.
    <x>, <y>: a pair of coordinates (can be any value)
*/
double perlin(double x, double y, char r);

/*
    Applies Perlin noise to the luminance of the image
    <surface>: the surface to be processed
*/
void noise(SDL_Surface *surface);

/*
    Applies Gaussian noise to the luminance of the image
    <surface>: the surface to be processed
    <deviation>: the deviation of the Gauss probability
    density function. 6 is generally considered a proper
    value for random noise distribution
    <intensity>: a multiplier applied to the noise
    intensity. Can be any value (+/-) but high/low
    values make the image fully bright/dark
*/
void gaussian_noise(SDL_Surface *surface, double deviation, double intensity);

/*
    Applies Gaussian noise to the hue of the image
    <surface>: the surface to be processed
    <deviation>: the deviation of the Gauss probability
    density function. 6 is generally considered a proper
    value for random noise distribution
    <intensity>: a multiplier applied to the noise
    intensity. Can be any value (+/-) but high/low
    values can mess up the color space
*/
void color_noise(SDL_Surface *surface, double deviation, double intensity);

#endif