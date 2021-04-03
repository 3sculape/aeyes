#include <stdio.h>
#include <stdlib.h>
#include "color_histogram.h"
#include <err.h>

#define COEFF 0.39215686

SDL_Surface* resize2(SDL_Surface *surface, double factor)
{
    return rotozoomSurface(surface, 0, factor, 1);
}

void color_histogram(SDL_Surface* surface, size_t red[], size_t green[],
        size_t blue[])
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
    }
}

SDL_Surface* show_histogram(SDL_Surface *surface)
{
    // Creates the output image
    SDL_Surface* histo = create_surface(250, 100);

    // calculates the factor in function of the image size
    double factor = 250.0 / (double)surface->w;
    //Temporary image of the size of the box
    SDL_Surface* tmp = resize2(surface, factor);
    size_t red[250];
    size_t green[250];
    size_t blue[250];
    //create histo
    color_histogram(tmp, red, green, blue);
    SDL_FreeSurface(tmp);
    for(int i = 0; i < 250; i++)
    {
        int j = 100;
        // scales the rgb values from 255 to 100
        int ri = (int)(COEFF * (double)red[i]);
        int gi = (int)(COEFF * (double)green[i]);
        int bi = (int)(COEFF * (double)blue[i]);

        if (!ri && !gi && !bi)
        {
            for(; j > 0; --j)
            {
                set_pixel(histo, 0, 0, 0, 0, i, j);
            }
            continue;
        }
        for(; j > 0; --j)
        {
            Uint8 r, g, b, a;
            r = 0;
            g = 0;
            b = 0;
            a = 0;
            if (ri)
            {
                r = 255;
                ri -= 1;
            }
            if (gi)
            {
                g = 255;
                gi -= 1;
            }
            if (bi)
            {
                b = 255;
                bi -= 1;
            }
            a = r || g || b;
            set_pixel(histo, r, g, b, a, i, j);
        }
    }
    return histo;
}
