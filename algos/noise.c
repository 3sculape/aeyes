#include "noise.h"

size_t hash(size_t x, size_t y)
{
    x = (x << 16) | (y & 0xffff);

    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;

    return x;
}

double grad(size_t corner_x, size_t corner_y, double dist_x, double dist_y)
{
        double pi_4 = cos(3.14 / 4.0);

        switch (hash(corner_x, corner_y) & 7)
        {
            case 0:
                return +dist_x;
            case 1:
                return +dist_y;
            case 2:
                return -dist_x;
            case 3:
                return -dist_y;
            case 4:
                return dist_x * pi_4 + dist_y * pi_4;
            case 5:
                return -dist_x * pi_4 + dist_y * pi_4;
            case 6:
                return -dist_x * pi_4 - dist_y * pi_4;
            case 7:
                return dist_x * pi_4 - dist_y * pi_4;
            default:
                warnx("grad function fail in noise");
                return 0.0;
        }
}

double perlin(double x, double y)
{
    size_t X = (size_t)floor(x) & 255;
    size_t Y = (size_t)floor(y) & 255;

    x -= floor(x);
    y -= floor(y);

    double grad00 = grad(X, Y, x, y);
    double grad01 = grad(X, Y + 1, x, y - 1.0);
    double grad10 = grad(X + 1, Y, x - 1.0, y);
    double grad11 = grad(X + 1, Y + 1, x - 1.0, y - 1.0);

    double u = fade(x);
    double v = fade(y);

    return (lerp(lerp(grad00, grad10, u), lerp(grad01, grad11, u), v) + 1) / 2;
}

double octave_perlin(double x, double y, char octaves, double persist)
{
    double frequency = 1;
    double amplitude = 1;
    double total = 0;
    double maxval = 0;

    for (int o = 0; o < octaves; o++)
    {
        total += perlin(x * frequency, y * frequency) * amplitude;
        maxval += amplitude;
        amplitude *= persist;
        frequency *= 2;
    }

    return total / maxval;
}

void noise_apply(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in noise_apply");
        return;
    }

    double noise;
    double hsv[3];
    Uint8 rgb[3];
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            noise = octave_perlin(i / 50.0, j / 50.0, 4, 1);
            rgb_to_hsv(r, g, b, hsv);
            hsv[2] += noise * 20.0;
            hsv[2] = clamp(hsv[2], 0, 100);
            hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb);

            set_pixel(surface, rgb[0], rgb[1], rgb[2], a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}