#include "color.h"

void brightest_pixel(SDL_Surface *surface, Uint8 *reference)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in brightest_pixel");
        return;
    }

    Uint8 max = 0;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 average = (r + g + b) / 3;

            if (average > max)
            {
                max = average;
                reference[0] = r;
                reference[1] = g;
                reference[2] = b;
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void white_balance(SDL_Surface *surface, Uint8 *reference)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    gsl_matrix *m = gsl_matrix_calloc(3, 3);
    gsl_vector *c = gsl_vector_calloc(3);
    gsl_vector *res = gsl_vector_calloc(3);

    gsl_matrix_set(m, 0, 0, 255 / reference[0]);
    gsl_matrix_set(m, 1, 1, 255 / reference[1]);
    gsl_matrix_set(m, 2, 2, 255 / reference[2]);
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            c -> data[0] = r;
            c -> data[1] = g;
            c -> data[2] = b;

            gsl_blas_dgemv(CblasNoTrans, 1, m, c, 1, res);

            set_pixel(surface, res -> data[0], res -> data[1], res -> data[2],
             a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
    gsl_matrix_free(m);
    gsl_vector_free(c);
    gsl_vector_free(res);
}