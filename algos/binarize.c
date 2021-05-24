#include "binarize.h"

void binarization(SDL_Surface *surface, Uint8 threshold, int ra, int ga,
int ba, int rb, int gb, int bb)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in binarization");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 average;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            average = (r + g + b) / 3;

            if (average > threshold)
            {
                r = ra;
                g = ga;
                b = ba;
            }

            else
            {
                r = rb;
                g = gb;
                b = bb;
            }

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void otsu_binarization(SDL_Surface *surface, Uint8 *color, Uint8 *color2)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in otsu_binarization");
        return;
    }

    Uint32 threshold;
    threshold = otsu_threshold(surface);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 average;

            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            average = (r + g + b) / 3;

            if (average > threshold)
            {
                r = color[0];
                g = color[1];
                b = color[2];
            }

            else
            {
                r = color2[0];
                g = color2[1];
                b = color2[2];
            }

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void gaussian_binarization(SDL_Surface *surface, int x, int c, Uint8 *color,
Uint8 *color2)
{
    if(SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in gaussian_binarization");
        return;
    }

    if(x % 2 == 0)
        x += 1;

    fast_gaussian_blur(surface, x);

    double sum;
    double sigma = 0.3 * ((x - 1) * 0.5 - 1) + 0.8;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    gsl_matrix *filter = gaussian_filter(x, sigma, &sum);
    Uint32 *matrix = (Uint32 *)malloc(sizeof(Uint32) * x * x);
    Uint32 average;
    Uint32 threshold;

    for(int j = 0; j < surface -> h; j++)
    {
        for(int i = 0; i < surface -> w; i++)
        {
            Uint8 r, g, b, a;
            Uint8 r2, g2, b2, a2;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            average = (r + g + b) / 3;

            get_pixel_around_x(surface, matrix, i, j, x);
            gaussian_average((size_t)x, sum, matrix, filter,
                    surface, &r2, &g2, &b2, &a2);
            threshold = (r2 + g2 + b2) / 3 - c;

            if (average > threshold)
            {
                r = color[0];
                g = color[1];
                b = color[2];
            }

            else
            {
                r = color2[0];
                g = color2[1];
                b = color2[2];
            }

            set_pixel(surface2, r, g, b, a, i, j);
        }
    }

    copy_surface(surface2, surface);
    free(matrix);
    gsl_matrix_free(filter);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}