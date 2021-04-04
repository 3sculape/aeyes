#include "../utils/lib_sdl.h"
#include <stdio.h>
#include <err.h>
#include "unsharp_masking.h"
#include "blurs.h"

int clamp(int x, int min, int max)
{
    if(x < min)
        return min;
    else if(x > max)
        return max;
    else
        return x;
}

SDL_Surface* sharpen(SDL_Surface *surface, double force, int threshold)
{
    if (surface == NULL)
        goto error;
    SDL_Surface* res = create_surface(surface->w, surface->h);
    if (res == NULL)
        goto error;

    SDL_Surface* org = create_surface(surface->w, surface->h);
    copy_surface(surface, org);
    if (org == NULL)
        goto error;
    gaussian_blur(org, 5, 5);
    saveJPG("./g.jpg", org);

    for (int i = 0; i < surface->w; i++)
    {
        for (int j = 0; j < surface->h; j++)
        {
            Uint8 or, og, ob, a;
            or = 0;
            og = 0;
            ob = 0;
            a = 0;
            Uint8 br, bg, bb;
            br = 0;
            bg = 0;
            bb = 0;

            Uint32 pixelblr = get_pixel(org, i, j);
            Uint32 pixelorg = get_pixel(surface, i, j);
            SDL_GetRGBA(pixelorg, org->format, &or, &og, &ob, &a);
            SDL_GetRGB(pixelblr, surface->format, &br, &bg, &bb);
            int r, g, b;
            r = (int)or + (((int)or - (int)br) * force);
            g = (int)og + (((int)og - (int)bg) * force);
            b = (int)ob + (((int)ob - (int)bb) * force);
            r = clamp(r, 0, 255);
            g = clamp(g, 0, 255);
            b = clamp(b, 0, 255);
            r = (int)or - (int)r > threshold ? or : r;
            g = (int)or - (int)g > threshold ? og : g;
            b = (int)or - (int)b > threshold ? ob : b;
            set_pixel(res, (Uint8)r, (Uint8)g, (Uint8)b, a, i, j);
        }
    }
    SDL_FreeSurface(org);
    return res;
error: warnx("unsharp mask function fail");
       return NULL;
}
