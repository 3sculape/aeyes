#include "../utils/lib_sdl.h"
#include <stdio.h>
#include <err.h>
#include "unsharp_masking.h"

SDL_Surface* sharpen(SDL_Surface *surface, double force)
{
    SDL_Surface* res = create_surface(surface->w, surface->h);
    gsl_matrix* kernel = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(kernel, 0, 0, 0);
    gsl_matrix_set(kernel, 0, 1, -1 * force);
    gsl_matrix_set(kernel, 0, 2, 0);
    gsl_matrix_set(kernel, 1, 0, -1 * force);
    gsl_matrix_set(kernel, 1, 1, 5);
    gsl_matrix_set(kernel, 1, 2, -1 * force);
    gsl_matrix_set(kernel, 2, 0, 0);
    gsl_matrix_set(kernel, 2, 1, -1 * force);
    gsl_matrix_set(kernel, 2, 2, 0);
    for (int i = 0; i < surface->w; i++)
    {
        for (int j = 0; j < surface->h; j++)
        {
            Uint8 r, g, b, a;
            Uint8 nr, ng, nb;
            r = 0;
            g = 0;
            b = 0;
            a = 0;
            nr = 0;
            ng = 0;
            nb = 0;
            for (int kj = -1; kj < 2; kj++)
            {
                for (int ki = -1; ki < 2; ki++)
                {
                    Uint32 pixel = get_pixel(surface, i, j);
                    if (pixel == (Uint32)-1)
                        goto error;
                    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                    //printf("kernel %f\n", (gsl_matrix_get(kernel, kj + 1, ki + 1)) * r);
                    nr = (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * r);
                    ng = (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * g);
                    nb = (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * b);
                }
            }
            set_pixel(surface, nr, ng, nb, a, i, j);
        }
    }
    return res;

error: warnx("function fail");
       return NULL;
}
