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