#ifndef UTILITY_H
#define UTILITY_H
#include <gsl/gsl_matrix.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "../utils/lib_sdl.h"
#include "basic.h"

/*
    Prints the matrix passed as argument
    <m>: the matrix to be printed
*/
void print_matrix(gsl_matrix *m);

/*
    Prints the vector passed as argument
    <v>: the vector to be printed
*/
void print_vector(gsl_vector *v);

/*
    Clamps x between min and max
    <x>: the value that needs clamping
    <min>: the clamping lower bound
    <max>: the clamping upper bound
*/
double clamp(double x, double min, double max);

/*
    Compute the distance between a and b
    <ax, ay>: the x and y coordinates of point a
    <bx, by>: the x and y coordinates of point b
*/
double distance(int ax, int ay, int bx, int by);

/*
    Linearly interpolate a and b with weight w
    (higher values of w bias the lerp towards a)
    <a>: the first point of interpolation
    <b>: the second point of interpolation
    <w>: the weight applied to interpolation
*/
double lerp(double a, double b, double w);

double blerp(double a, double b, double c, double d, double tx, double ty);

/*
    Fade function as defined by Ken Perlin:
    f(t) = 6t^5 - 15t^4 + 10t^3
    <t>: the double on which to apply fading
*/
double fade(double t);

int otsu_threshold(SDL_Surface* org);

void get_pixel_around_x(SDL_Surface *surface, Uint32 *matrix,
        int posx, int posy, int x);

gsl_matrix *gaussian_filter(int x, double sigma, double *sum);

void gaussian_average(size_t x, double sum, Uint32 *matrix, gsl_matrix *filter,
        SDL_Surface *surface, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);

#endif
