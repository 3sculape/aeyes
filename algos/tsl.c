#include "tsl.h"

void tsl_hue(SDL_Surface *surface, int color, double amount, double range)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in tsl_hue");
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

            double x = (hsv[0] - color) * (4.0 / range);
            smooth = gsl_ran_ugaussian_pdf(x / 2) * 2.5;
            hsv[0] = fmod((hsv[0] + amount * smooth), 360);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);
            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void tsl_sat(SDL_Surface *surface, int color, double amount, double range)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in tsl_sat");
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

            double x = (hsv[0] - color) * (4.0 / range);
            smooth = gsl_ran_ugaussian_pdf(x / 2) * 2.5;
            hsv[1] = clamp(hsv[1] + amount * smooth, 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);
            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void tsl_val(SDL_Surface *surface, int color, double amount, double range)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in tsl_light");
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

            double x = (hsv[0] - color) * (4.0 / range);
            smooth = gsl_ran_ugaussian_pdf(x / 2) * 2.5;
            hsv[2] = clamp(hsv[2] + amount * smooth, 0, 100);

            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);
            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}