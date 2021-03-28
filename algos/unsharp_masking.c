#include "../utils/lib_sdl.h"
#include <stdio.h>
#include <err.h>
#include "unsharp_masking.h"
#include "blurs.h"

SDL_Surface* sharpen(SDL_Surface *surface, double force)
{
    if (surface == NULL)
        goto error;
    SDL_Surface* res = create_surface(surface->w, surface->h);
    if (res == NULL)
        goto error;

    SDL_Surface* org = create_surface(surface->w, surface->h);
    copy_surface(surface, org);
    saveJPG("org.JPG", org);
    if (org == NULL)
        goto error;
    gaussian_blur(surface, 5);
    saveJPG("blur.JPG", surface);

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

            Uint32 pixelorg = get_pixel(org, i, j);
            Uint32 pixelblr = get_pixel(surface, i, j);

            SDL_GetRGBA(pixelorg, org->format, &or, &og, &ob, &a);
            SDL_GetRGB(pixelblr, surface->format, &br, &bg, &bb);
            Uint8 r, g, b;
            r = or + ((or - br) * force);
            g = og + ((og - bg) * force);
            b = ob + ((ob - bb) * force);
            set_pixel(res, r, g, b, a, i, j);
        }
    }
    SDL_FreeSurface(org);
    return res;
error: warnx("unsharp mask function fail");
       return NULL;
}
