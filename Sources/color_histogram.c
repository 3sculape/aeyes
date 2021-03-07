#include <stdio.h>
#include <stdlib.h>
#include "../Header/lib_sdl.h"
#include "../Header/color_histogram.h"
#include <err.h>

void print_vector(size_t arr[], size_t len)
{
    printf("[%lu", arr[0]);
    for(size_t i = 1; i < len; i++)
    {
        printf(",%lu", arr[i]);
    }
    printf("]\n");
}

void color_histogram(SDL_Surface* surface, size_t red[], size_t green[], size_t blue[])
{
    size_t maxh = surface->h;
    size_t maxw = surface->w;
    for(size_t width = 0; width < maxw; width++)
    {
        Uint8        r, g, b;
        size_t       avgr = 0;
        size_t       avgg = 0;
        size_t       avgb = 0;

        for(size_t height = 0; height < maxh; height++)
        {
            Uint32  pixel;
            /*if ((int)*/(pixel = get_pixel(surface, width, height));/* == -1)
                errx(EXIT_FAILURE, "get_pixel returned -1");*/
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            avgr += (size_t) r;
            avgg += (size_t) g;
            avgb += (size_t) b;
        }
        avgr /= maxh;
        avgg /= maxh;
        avgb /= maxh;
        red[width] = avgr;
        green[width] = avgg;
        blue[width] = avgb;
        //printf("Column %lu |r:%lu, g:%lu, b:%lu|\n", width, avgr, avgg, avgb);
    }
    printf("red:\n");
    print_vector(red, maxw);
    printf("green:\n");
    print_vector(green,maxw);
    printf("blue:\n");
    print_vector(blue, maxw);
}
