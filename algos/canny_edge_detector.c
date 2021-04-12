#include "canny_edge_detector.h"

SDL_Surface* canny(SDL_Surface *surface)
{
    SDL_Surface* copy = create_surface(surface->w, surface->h);
    SDL_Surface* canny = create_surface(surface->w, surface->h);
    gaussian_blur(copy, 5, 5);

    gsl_matrix* Gx = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gx, 0, 0, -1);
    gsl_matrix_set(Gx, 0, 1, 0);
    gsl_matrix_set(Gx, 0, 2, 1);
    gsl_matrix_set(Gx, 1, 0, -2);
    gsl_matrix_set(Gx, 1, 1, 0);
    gsl_matrix_set(Gx, 1, 2, 2);
    gsl_matrix_set(Gx, 2, 0, -1);
    gsl_matrix_set(Gx, 2, 1, 0);
    gsl_matrix_set(Gx, 2, 2, 1);

    gsl_matrix* Gy = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gy, 0, 0, 1);
    gsl_matrix_set(Gy, 0, 1, 2);
    gsl_matrix_set(Gy, 0, 2, 1);
    gsl_matrix_set(Gy, 1, 0, 0);
    gsl_matrix_set(Gy, 1, 1, 0);
    gsl_matrix_set(Gy, 1, 2, 0);
    gsl_matrix_set(Gy, 2, 0, -1);
    gsl_matrix_set(Gy, 2, 1, -2);
    gsl_matrix_set(Gy, 2, 2, -1);

    gsl_matrix_free(Gx);
    gsl_matrix_free(Gy);
    SDL_FreeSurface(copy);
    return canny;
}
