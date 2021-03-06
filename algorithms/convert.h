#ifndef CONVERT_H
#define CONVERT_H
#include <SDL2/SDL.h>

/*
    Convert an RGB value into its HSV (Hue, Saturation, Value)
    equivalent. Notice parameters are not pointers but direct
    values. <*hsv> is updated with H at index 0, S at index 1
    and V at index 2. 
    WARNING: since the size of <*hsv> cannot be checked, the
    program will not compile if the provided size is too small
    <r>: the pixel's red value
    <g>: the pixel's green value
    <b>: the pixel's blue value
    <*hsv>: an array with the newly computed H, S and V values
*/
void rgb_to_hsv(Uint8 r, Uint8 g, Uint8 b, double *hsv);

/*
    Convert an HSV value into an RGB value.
    This function is subject to the same restrictions as above
    NOTICE: the function hsv_to_rgb_sub should not be directly
    called outside of this file!
    <h>: the pixel's hue value
    <s>: the pixel's saturation value
    <v>: the pixel's brightness value
    <*rgb>: an array with the newly computed R, G and B values
*/
void hsv_to_rgb(double h, double s, double v, double *rgb);

#endif