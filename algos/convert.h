#ifndef CONVERT_H
#define CONVERT_H
#include <SDL2/SDL.h>
#include <math.h>

/*
    Convert an RGB value into its HSV (Hue, Saturation, Value)
    equivalent. <*hsv> is updated with H at index 0, S at
    index 1 and V at index 2. 
    H is in range [0; 360]
    S is in range [0; 100]
    V is in range [0; 100]
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
void hsv_to_rgb(double h, double s, double v, Uint8 *rgb);

/*
    Convert an RGB value into its HSL (Hue, Saturation, Lightness)
    equivalent. <*hsl> is updated with H at index 0, S at
    index 1 and L at index 2. 
    H is in range [0; 360]
    S is in range [0; 100]
    L is in range [0; 100]
    WARNING: since the size of <*hsl> cannot be checked, the
    program will not compile if the provided size is too small
    <r>: the pixel's red value
    <g>: the pixel's green value
    <b>: the pixel's blue value
    <*hsl>: an array with the newly computed H, S and L values
*/
void rgb_to_hsl(Uint8 r, Uint8 g, Uint8 b, double *hsl);

/*
    Convert an HSL value into an RGB value.
    <h>: the pixel's hue value
    <s>: the pixel's saturation value
    <l>: the pixel's lightness value
    <*rgb>: an array with the newly computed R, G and B values
*/
void hsl_to_rgb(double h, double s, double l, Uint8 *rgb);

#endif