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

void white_balance(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    Uint8 color[3];
    if (factor < -1 || factor > 1 || factor == 0)
        return;

    if (factor < 0)
    {
        color[0] = 0;
        color[1] = 67;
        color[2] = 255;
    }

    else
    {
        color[0] = 255;
        color[1] = 138;
        color[2] = 0;
    }

    factor = fabs(factor);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            double old_hsl[3];
            double new_hsl[3];
            Uint8 rgb[3];

            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsl(r, g, b, old_hsl);
            r = (r * (color[0] + (255 - color[0]) * (1 - factor))) / 255;
            g = (g * (color[1] + (255 - color[1]) * (1 - factor))) / 255;
            b = (b * (color[2] + (255 - color[2]) * (1 - factor))) / 255;

            rgb_to_hsl(r, g, b, new_hsl);
            new_hsl[2] = old_hsl[2];
            hsl_to_rgb(new_hsl[0], new_hsl[1], new_hsl[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void tint(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    Uint8 color[3];
    if (factor < -1 || factor > 1 || factor == 0)
        return;

    if (factor < 0)
    {
        color[0] = 0;
        color[1] = 203;
        color[2] = 0;
    }

    else
    {
        color[0] = 203;
        color[1] = 0;
        color[2] = 203;
    }

    factor = fabs(factor);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            double old_hsl[3];
            double new_hsl[3];
            Uint8 rgb[3];

            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsl(r, g, b, old_hsl);
            r = (r * (color[0] + (255 - color[0]) * (1 - factor))) / 255;
            g = (g * (color[1] + (255 - color[1]) * (1 - factor))) / 255;
            b = (b * (color[2] + (255 - color[2]) * (1 - factor))) / 255;

            rgb_to_hsl(r, g, b, new_hsl);
            new_hsl[2] = old_hsl[2];
            hsl_to_rgb(new_hsl[0], new_hsl[1], new_hsl[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}