#ifndef BASIC_H
#define BASIC_H
#include <SDL2/SDL.h>
#include <err.h>
#include "../utils/lib_sdl.h"
#include "convert.h"
#include <SDL2/SDL2_rotozoom.h>
#include "utility.h"

/*
    All algorithms provided in this header do not
    return; instead, they modify the provided SDL
    surface in-place. The pointer is thus set
    directly to the processed image which prevents
    unneeded address copying
*/

/*
    Compute the grayscale by iterating over each pixel
    and applying a formula based on its RGB value
    <surface>: the surface to be processed
*/
void grayscale(SDL_Surface *surface);

/*
    Negative of the image. For each pixel, substract its current
    red value from 255 to obtain its new red value, and repeat for
    all colors.
    <surface>: the surface to be processed
*/
void negative(SDL_Surface *surface);

/*
    Colorization of the image. Each pixel is converted to
    the color defined by the triplet {ra, ga, ba}. If
    <keep_luminance> is set, each pixel will keep its
    original L value.
    <surface>: the surface to be processed
    <threshold>: the threshold for binarization
    {ra, ga, ba}: a standard RGB color
    <keep_luminance>: a boolean
*/
void colorize(SDL_Surface *surface, int ra, int ga, int ba, int keep_luminance);

/*
    Image clipping. Turn all fully white pixels into red, and turn
    all fully black pixels into blue.
    <surface>: the surface to be processed
*/
void clipping(SDL_Surface *surface);

/*
    Saturation control. Converts each pixel into HSL, then changes
    the value to the <value> parameter.
    <surface>: the surface to be processed
    <value>: the new saturation (S) value
*/
void saturation(SDL_Surface *surface, double value);

/*
    Exposure control. Converts each pixel into HSL, then changes
    the value to the <value> parameter.
    <surface>: the surface to be processed
    <value>: the new exposure (L) value
*/
void exposure(SDL_Surface *surface, double value);

/*
    Contrast control. Changes the saturation of each pixel to the
    <value> parameter multiplied with a contrast factor.
    <surface>: the surface to be processed
    <value>: the new saturation value
*/
void contrast(SDL_Surface *surface, double value);

/*
    Basic image cropping, takes into parameter the source we want to copy from,
    the starting position of the left corner and the width and height of the
    wanted section. Returns the texture create from it
*/
SDL_Surface* image_crop(SDL_Surface* original, size_t x, size_t y, size_t w,
        size_t h);

/*
 * Basic image resize, wrapper to rotozoom for practicality
 */
SDL_Surface* resize(SDL_Surface* original, double factor);

/*
 * Basic image rotate, wrapper to rotozoom for practicality
 */
SDL_Surface* rotate(SDL_Surface* original, double angle);

// Mirror flip based on axis, xaxis is bool for if flip is on xaxis or nah
void mirror(SDL_Surface *surface, int xaxis);

void mean(SDL_Surface* surface);

// Doesn't work for now
SDL_Surface *resize_fit_to_scale(SDL_Surface* original, double factor);

SDL_Surface *crop_from_center(SDL_Surface* original, size_t w, size_t h);

// Zoom function, calls resize but retrieves the original size of the image,
// future upgrade -> being able to choose where to zoom in, right now only
// center of the image, factor > 0
SDL_Surface* zoom(SDL_Surface* original, double factor);
// scale the image to the size w, h by streching it.
SDL_Surface* scale_strech(SDL_Surface *surface, int w, int h);

// Symmetry function, second param is choice for axis (1 for y axis, 0 for x)
// third param is bool to change direction
void symmetry(SDL_Surface* original, int y_axis, int direction);

// offset image from left to right (like caml) by default, bool axis for top
// to bottom by int amount
void offset(SDL_Surface* original, int amount, int axis);

#endif
