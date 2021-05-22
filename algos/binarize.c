#include "binarize.h"

void binarization(SDL_Surface *surface, Uint32 threshold, int ra, int ga,
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

void colorize(SDL_Surface *surface, int ra, int ga, int ba, int keep_luminance)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in binarization");
        return;
    }

    double hsv_input[3];
    rgb_to_hsl(ra, ga, ba, hsv_input);

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            Uint8 rgb[3];
            double hsv2[3];
            rgb_to_hsl(r, g, b, hsv2);

            hsv2[0] = hsv_input[0];
            hsv2[1] = hsv_input[1];

            if (keep_luminance)
            {
                hsv2[2] = hsv2[2]/50 * hsv_input[2];
                if (hsv2[2] > 100)
                {
                    hsv2[2] = 100;
                }
            }

            else
            {
                hsv2[2] = hsv2[2]/100 * hsv_input[2];
            }

            hsl_to_rgb(hsv2[0], hsv2[1], hsv2[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}