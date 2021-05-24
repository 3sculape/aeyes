#include "../utils/lib_sdl.h"
#include "effects.h"
#include "basic.h"
#include "utility.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <math.h>

void swirl(SDL_Surface *surface, double factor)
{
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    int w = surface->w;
    int h = surface->h;
    int midx = w / 2;
    int midy = h / 2;

    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            int dx = i - midx;
            int dy = j - midy;
            double theta = atan2(dy, dx);
            double radius = sqrt(dx * dx + dy * dy);
            double newx = midx +
                (radius * cos(theta + (factor * radius * M_PI) / 180));
            double newy = midy +
                (radius * sin(theta + (factor * radius * M_PI) / 180));
            if(newx > 0 && newx < w && newy > 0 && newy < h)
            {
                Uint32 pixel = get_pixel(surface, newx, newy);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(surface2, r, g, b, a, i, j);
            }
        }
    }


    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void twist(SDL_Surface *surface, double factor)
{
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    int w = surface->w;
    int h = surface->h;

    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            float dx = (factor * sin(2 * M_PI * j / 64));
            float dy = (factor * sin(2 * M_PI * i / 64));

            float newx = i + dx;
            float newy = j + dy;

            if(newx > 0 && newx < w && newy > 0 && newy < h)
            {
                Uint32 pixel = get_pixel(surface, newx, newy);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(surface2, r, g, b, a, i, j);
            }
        }
    }
    SDL_Surface *surface3 = image_crop(surface2, (size_t)factor + 1,
            (size_t)factor + 1, surface2->w - 2 * (size_t)factor - 1,
            surface2->h - 2 * (size_t)factor - 1);

    SDL_Surface * surface4 = scale_strech(surface3, surface->w, surface->h);

    copy_surface(surface4, surface);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surface4);
    SDL_UnlockSurface(surface);
}

void solve(gsl_matrix *A, gsl_vector *b, gsl_vector *v)
{
    int signum;
    gsl_permutation *p = gsl_permutation_alloc(A->size1);
    gsl_matrix *tmpA = gsl_matrix_alloc(A->size1, A->size2);
    gsl_matrix_memcpy(tmpA , A);

    gsl_linalg_LU_decomp(tmpA, p, &signum);
    gsl_linalg_LU_solve(tmpA, p, b, v);
    gsl_matrix_free(tmpA);
    gsl_permutation_free(p);
}

SDL_Surface *perspective(SDL_Surface * surface, SDL_Point **src, SDL_Point **dst)
{
    gsl_matrix *A = gsl_matrix_calloc(8, 8);
    gsl_vector *b = gsl_vector_calloc(8);
    for(int i = 0; i < 4; i++)
    {
        gsl_matrix_set(A, i, 0, src[i]->x);
        gsl_matrix_set(A, i + 4, 3, src[i]->x);
        gsl_matrix_set(A, i, 1, src[i]->y);
        gsl_matrix_set(A, i + 4, 4, src[i]->y);
        gsl_matrix_set(A, i, 2, 1);
        gsl_matrix_set(A, i + 4, 5, 1);
        gsl_matrix_set(A, i, 6, -src[i]->x * dst[i]->x);
        gsl_matrix_set(A, i, 7, -src[i]->y * dst[i]->x);
        gsl_matrix_set(A, i + 4, 6, -src[i]->x * dst[i]->y);
        gsl_matrix_set(A, i + 4, 7, -src[i]->y * dst[i]->y);
        gsl_vector_set(b, i, dst[i]->x);
        gsl_vector_set(b, i + 4, dst[i]->y);
    }


    gsl_vector *v = gsl_vector_calloc(8);
    solve(A, b, v);
    gsl_matrix_free(A);
    gsl_vector_free(b);

    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);

    for(int i = 0; i < surface -> w; i++)
    {
        for(int j = 0; j < surface -> h; j++)
        {
            int x = round((gsl_vector_get(v, 0) * i +
                    gsl_vector_get(v, 1) * j +
                    gsl_vector_get(v, 2)) /
                (gsl_vector_get(v, 6) * i +
                 gsl_vector_get(v, 7) * j +
                 1));
            int y = round((gsl_vector_get(v, 3) * i +
                    gsl_vector_get(v, 4) * j +
                    gsl_vector_get(v, 5)) /
                (gsl_vector_get(v, 6) * i +
                 gsl_vector_get(v, 7) * j +
                 1));

            Uint32 pixel = get_pixel(surface, x, y);
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            set_pixel(surface2, r, g, b, a, i, j);
        }
    }

    gsl_vector_free(v);

    return surface2;
}
