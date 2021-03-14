#include "../utils/lib_sdl.h"
#include "blurs.h"
#include <gsl/gsl_matrix.h>
#include <math.h>


void get_pixel_around_3(SDL_Surface *surface, Uint32 *matrix, int posx, int posy)
{
    for(int j = posy - 1; j <= posy + 1; j++)
    {
        for(int i = posx - 1; i <= posx + 1; i++)
        {
            if(j < 0)
            {
                if(i < 0)
                {
                    matrix[(j - posy + 1) * 3 + i - posx + 1] =
                        get_pixel(surface, i + 1, j + 1);
                }
                else if(i >= surface->w)
                {
                    matrix[(j - posy + 1) * 3 + i - posx + 1] =
                        get_pixel(surface, i - 1, j + 1);
                }
                else
                {
                    matrix[(j - posy + 1) * 3 + i - posx + 1] =
                        get_pixel(surface, i, j + 1);
                }
            }

            else if(j >= surface->h)
            {
                if(i < 0)
                {
                    matrix[(j - posy + 1) * 3 + i - posx + 1] =
                        get_pixel(surface, i + 1, j - 1);
                }
                else if(i >= surface->w)
                {
                    matrix[(j - posy + 1) * 3 + i - posx + 1] =
                        get_pixel(surface, i - 1, j - 1);
                }
                else
                {
                    matrix[(j - posy + 1) * 3 + i - posx + 1] =
                        get_pixel(surface, i, j - 1);
                }
            }

            else if(i < 0)
            {
                matrix[(j - posy + 1) * 3 + i - posx + 1] =
                    get_pixel(surface, i + 1, j);
            }
            else if(i >= surface->w)
            {
                matrix[(j - posy + 1) * 3 + i - posx + 1] =
                    get_pixel(surface, i - 1, j);
            }
            else
            {
                matrix[(j - posy + 1) * 3 + i - posx + 1] =
                    get_pixel(surface, i, j);
            }
        }
    }
}

void get_pixel_around_x(SDL_Surface *surface, Uint32 *matrix, int posx, int posy, size_t x)
{
    for(int j = posy - (x - 1) / 2; j <= posy + (x - 1) / 2; j++)
    {
        for(int i = posx - (x - 1) / 2; i <= posx + (x - 1) / 2; i++)
        {
            if(j < 0)
            {
                if(i < 0)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                        get_pixel(surface, i + (x - 1) / 2, j + (x - 1) / 2);
                }
                else if(i >= surface->w)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                        get_pixel(surface, i - (x - 1) / 2, j + (x - 1) / 2);
                }
                else
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                        get_pixel(surface, i, j + (x - 1) / 2);
                }
            }

            else if(j >= surface->h)
            {
                if(i < 0)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                        get_pixel(surface, i + (x - 1) / 2, j - (x - 1) / 2);
                }
                else if(i >= surface->w)
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                        get_pixel(surface, i - (x - 1) / 2, j - (x - 1) / 2);
                }
                else
                {
                    matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                        get_pixel(surface, i, j - (x - 1) / 2);
                }
            }

            else if(i < 0)
            {
                matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                    get_pixel(surface, i + (x - 1) / 2, j);
            }
            else if(i >= surface->w)
            {
                matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                    get_pixel(surface, i - (x - 1) / 2, j);
            }
            else
            {
                matrix[(j - posy + (x - 1) / 2) * x + i - posx + (x - 1) / 2] =
                    get_pixel(surface, i, j);
            }
        }
    }
}


void get_average(SDL_Surface *surface, Uint32 *matrix, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
    int totr, totg, totb;
    totr = 0;
    totg = 0;
    totb = 0;
    for (int i = 0; i < 9; i++)
    {
        SDL_GetRGB(matrix[i], surface->format, r, g, b);
        totr += *r;
        totg += *g;
        totb += *b;
    }

    SDL_GetRGBA(matrix[4], surface->format, r, g, b, a);

    totr /= 9;
    totg /= 9;
    totb /= 9;

    *r = totr;
    *g = totg;
    *b = totb;
}

gsl_matrix *gaussian_filter(size_t x, double sigma, double *sum)
{
    gsl_matrix *matrix = gsl_matrix_calloc(x, x);
    *sum = 0;

    for(size_t j = -1 * (x - 1) / 2; j < (x - 1) / 2; j++)
    {
        for(size_t i = -1 * (x - 1) / 2; i < (x - 1) / 2; i++)
        {
            double d;
            d = exp(-1 * (i * i + j * j) / (2 * sigma * sigma));
            gsl_matrix_set(matrix, i + (x - 1) / 2, j + (x - 1) / 2, d);
            *sum += d;
        }
    }

    return matrix;
}

void gaussian_average(size_t x, double sum, Uint32 *matrix, gsl_matrix *filter)
{

}

void gaussian_blur(SDL_Surface *surface, size_t x)
{
    if(SDL_LockSurface(surface) != 0)
        return;
    double sum;
    gsl_matrix *filter = gaussian_filter(x, 0.5, &sum);
    Uint32 *matrix = (Uint32 *)malloc(sizeof(Uint32) * x * x);
    for(int j = 0; j < surface -> h; j++)
    {
        for(int i = 0; i < surface -> w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_around_x(surface, matrix, i, j, x);
            gaussian_average(x, sum, matrix, filter, )
            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}


void box_blur(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
        return;

    Uint32 *matrix = (Uint32 *)malloc(sizeof(Uint32) * 9);
    for(int j = 0; j < surface -> h; j++)
    {
        for(int i = 0; i < surface -> w; i++)
        {
            Uint8 r, g, b, a;
            get_pixel_around_3(surface, matrix, i, j);
            get_average(surface, matrix, &r, &g, &b, &a);
            set_pixel(surface, r, g, b, a, i, j);
        }
    }
    free(matrix);
    SDL_UnlockSurface(surface);
}
