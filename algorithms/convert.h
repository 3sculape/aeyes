#ifndef CONVERT_H
#define CONVERT_H
#include <SDL2/SDL.h>

/*
    Convert an RGB value into its HSV (Hue, Saturation, Value)
    equivalent. Notice parameters are not pointers but direct
    values. <*hsv> is updated with H at index 0, S at index 1
    and V at index 2. 
    WARNING: since the size of <*hsv> cannot be checked, the
    program will segfault if the provided size is too small
    <r>: the pixel's red value
    <g>: the pixel's green value
    <b>: the pixel's blue value
    <*hsv>: an array with the newly computed H, S and V values
*/
void rgb_to_hsv(Uint8 r, Uint8 g, Uint8 b, double *hsv);

#endif