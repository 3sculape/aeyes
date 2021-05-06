#include "vignette.h"

void vignette(SDL_Surface *surface, Uint8 *color, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in vignette");
        return;
    }

    factor /= 100.0;
    double strength;
    size_t middle[2] = {surface -> w / 2, surface -> h / 2};
    size_t max_dist = distance(0, 0, middle[0], middle[1]);
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            double dist = distance(i, j, middle[0], middle[1]);
            double rel_dist = max_dist - dist;
            strength = gsl_ran_ugaussian_pdf(rel_dist / (max_dist / 4)) * 2.5;
            r = r * (1 - strength * factor) + color[0] * (strength * factor);
            g = g * (1 - strength * factor) + color[1] * (strength * factor);
            b = b * (1 - strength * factor) + color[2] * (strength * factor);

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}