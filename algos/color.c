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

void white_balance(SDL_Surface *surface, size_t factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    Uint8 blue[3] = {0, 67, 255};
    Uint8 orange[3] = {255, 143, 0};
    Uint8 base_color[3];
    double base_factor;
    double og_factor;
    if (factor == 50)
        return;

    if (factor > 50)
    {
        base_color[0] = blue[0];
        base_color[1] = blue[1];
        base_color[2] = blue[2];
        base_factor = factor / 200;
    }

    else
    {
        base_color[0] = orange[0];
        base_color[1] = orange[1];
        base_color[2] = orange[2];
        base_factor = factor / 100;
    }

    og_factor = 1 - base_factor;

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            r = r * og_factor + base_color[0] * base_factor; 
            g = g * og_factor + base_color[1] * base_factor;
            b = b * og_factor + base_color[2] * base_factor;

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}