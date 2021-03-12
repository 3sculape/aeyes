#include "color.h"

void brightest_pixel(SDL_Surface *surface, Uint8 *reference)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in brightest_pixel");
        return;
    }

    Uint8 max = 0;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 average = (r + g + b) / 3;

            if (average < 250 && average > max)
            {
                max = average;
                reference[0] = r;
                reference[1] = g;
                reference[2] = b;
            }
        }
    }

    SDL_UnlockSurface(surface);
}
