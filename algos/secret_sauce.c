#include "secret_sauce.h"

void deep_fry(SDL_Surface *surface, Uint8 *reference)
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

            gsl_blas_dgemv(CblasNoTrans, 1, m, c, 0, res);

            set_pixel(surface, res -> data[0], res -> data[1], res -> data[2],
             a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
    gsl_matrix_free(m);
    gsl_vector_free(c);
    gsl_vector_free(res);
}

SDL_Surface* fever_dream(SDL_Surface *surface, double force)
{
    if (surface == NULL)
        goto error;
    SDL_Surface* res = create_surface(surface->w, surface->h);
    if (res == NULL)
        goto error;
    gsl_matrix* kernel = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(kernel, 0, 0, 0);
    gsl_matrix_set(kernel, 0, 1, -1 * force);
    gsl_matrix_set(kernel, 0, 2, 0);
    gsl_matrix_set(kernel, 1, 0, -1 * force);
    gsl_matrix_set(kernel, 1, 1, 5 * force);
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
                    /*if (pixel == -1)
                        goto error;*/
                    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                    nr += (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * r);
                    ng += (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * g);
                    nb += (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * b);
                }
            }
            set_pixel(res, nr, ng, nb, a, i, j);
        }
    }
    return res;

error: warnx("unsharp mask function fail");
       return NULL;
}
