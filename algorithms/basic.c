#include "basic.h"
#include <err.h>
#include "../Header/lib_sdl.h"

void grayscale(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in grayscale");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 total = 0.21 * r + 0.72 * g + 0.07 * b;
            set_pixel(surface, total, total, total, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}