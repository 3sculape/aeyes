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
    Each range is ?? degrees wide.
*/
enum color {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE};

/*
    A typedef is used so that the function can recieve colors
    directly without specifying the enum type.
*/
typedef enum color color_t;

/*
    Hue rotation. Gets all pixels in the range defined by <range>
    and increases their hue by <amount> modulo 360
    <surface>: the surface to be processed
    <range>: an array of 2 elements
    <amount>: the amount of rotation to be applied (can be anything)
    NOTES: 
        - If range has less than 2 elements, a segfault will occur.
        - If it has more, the remaining elements will be ignored.
        - Both values can be equal, hence targeting only a single color.
*/
void tsl_hue(SDL_Surface *surface, double *range, double amount);

/*
    Saturation modification. Gets all pixels in the range defined by <range>
    and increases their saturation by <amount> clamped to [0, 100]
    <surface>: the surface to be processed
    <range>: an array of 2 elements
    <amount>: the amount of saturation to apply (can be anything)
    NOTES: 
        - If range has less than 2 elements, a segfault will occur.
        - If it has more, the remaining elements will be ignored.
        - Both values can be equal, hence targeting only a single color.
*/
void tsl_sat(SDL_Surface *surface, double *range, double amount);

/*
    Value modification. Gets all pixels in the range defined by <range>
    and increases their value by <amount> clamped to [0, 100]
    <surface>: the surface to be processed
    <range>: an array of 2 elements
    <amount>: the amount of value to apply (can be anything)
    NOTES: 
        - If range has less than 2 elements, a segfault will occur.
        - If it has more, the remaining elements will be ignored.
        - Both values can be equal, hence targeting only a single color.
*/
void tsl_val(SDL_Surface *surface, double *range, double amount);

#endif