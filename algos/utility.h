#ifndef UTILITY_H
#define UTILITY_H
#include <gsl/gsl_matrix.h>
#include <stdio.h>
#include <math.h>

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

#endif
