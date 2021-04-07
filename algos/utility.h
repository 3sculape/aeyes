#ifndef UTILITY_H
#define UTILITY_H
#include <gsl/gsl_matrix.h>
#include <stdio.h>

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

#endif