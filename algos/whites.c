#include "whites.h"

void blacks(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in blacks");
        return;
    }

    double hsv[3];
    Uint8 rgb[3];
    double smooth;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsv(r, g, b, hsv);

            double x = hsv[2] * (4.0 / 10.0);
            smooth = fabs(gsl_ran_ugaussian_pdf(x / 2));
            hsv[2] += (factor / 10) * (smooth * 2.5) * hsv[2];

            hsv[2] = clamp(hsv[2], 0, 100);
            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void shadows(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in shadows");
        return;
    }

    double hsv[3];
    Uint8 rgb[3];
    double smooth;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsv(r, g, b, hsv);

            double x = hsv[2] * (4.0 / 10.0);
            smooth = fabs(gsl_ran_ugaussian_pdf(x / 3));
            hsv[2] += (factor / 50) * (smooth * 2.5) * hsv[2];

            hsv[2] = clamp(hsv[2], 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void highlights(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in highlights");
        return;
    }

    double hsv[3];
    Uint8 rgb[3];
    double smooth;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsv(r, g, b, hsv);

            double x = (100 - hsv[2]) * (4.0 / 10.0);
            smooth = fabs(gsl_ran_ugaussian_pdf(x / 3));
            hsv[2] += (factor / 10) * (smooth * 2.5) * (100 - hsv[2]);

            hsv[2] = clamp(hsv[2], 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void whites(SDL_Surface *surface, double factor)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in highlights");
        return;
    }

    double hsv[3];
    Uint8 rgb[3];
    double smooth;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);

            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            rgb_to_hsv(r, g, b, hsv);

            double x = (100 - hsv[2]) * (4.0 / 10.0);
            smooth = fabs(gsl_ran_ugaussian_pdf(x / 2));
            hsv[2] += (factor / 10) * (smooth * 2.5) * (100 - hsv[2]);

            hsv[2] = clamp(hsv[2], 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}