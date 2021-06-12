#ifndef TSL_H
#define TSL_H
#include <err.h>
#include <SDL2/SDL.h>
#include <gsl/gsl_randist.h>
#include "../utils/lib_sdl.h"
#include "convert.h"
#include "utility.h"

/*
    This enum defines the different color ranges that can be used
    with the functions below.
*/
enum color {RED = 0, ORANGE = 30, YELLOW = 60, GREEN = 90, TEAL = 165,
BLUE = 215, PURPLE = 270, MAGENTA = 300};

/*
    A typedef is used so that the function can recieve colors
    directly without specifying the enum type.
*/
typedef enum color color_t;

/*
    Hue rotation. Gets all pixels in the range defined by <range>
    and increases their hue by <amount> modulo 360
    <surface>: the surface to be processed
    <color>: a value as defined in the enum above or a custom value
    <amount>: the amount of rotation to be applied (can be anything)
    <range>: the range of pixels to target
*/
void tsl_hue(SDL_Surface *surface, int color, double amount, double range);

/*
    Saturation modification. Gets all pixels in the range defined by <range>
    and increases their saturation by <amount> clamped to [0, 100]
    <surface>: the surface to be processed
    <color>: a value as defined in the enum above or a custom value
    <amount>: the amount of saturation to apply (can be anything)
    <range>: the range of pixels to target
*/
void tsl_sat(SDL_Surface *surface, int color, double amount, double range);

/*
    Value modification. Gets all pixels in the range defined by <range>
    and increases their value by <amount> clamped to [0, 100]
    <surface>: the surface to be processed
    <color>: a value as defined in the enum above or a custom value
    <amount>: the amount of value to apply (can be anything)
    <range>: the range of pixels to target
*/
void tsl_val(SDL_Surface *surface, int color, double amount, double range);

#endif