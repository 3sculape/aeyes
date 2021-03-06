#include "convert.h"

double max(double a, double b)
{
    return (a > b) ? a : b;
}

double min(double a, double b)
{
    return (a < b) ? a : b;
}

void rgb_to_hsv(Uint8 r, Uint8 g, Uint8 b, double *hsv)
{
    double rgb[3];
    double max_val;
    double min_val;
    double diff;

    rgb[0] = r / 255.0;
    rgb[1] = g / 255.0;
    rgb[2] = b / 255.0;

    max_val = max(rgb[0], max(rgb[1], rgb[2]));
    min_val = min(rgb[0], min(rgb[1], rgb[2]));

    diff = max_val - min_val;

    if (max_val == min_val)
        hsv[0] = 0;

    else if (max_val == rgb[0])
        hsv[0] = fmod((60 * ((rgb[1] - rgb[2]) / diff) + 360), 360);

    else if (max_val == rgb[1])
        hsv[0] = fmod((60 * ((rgb[2] - rgb[0]) / diff) + 120), 360);

    else if (max_val == rgb[2])
        hsv[0] = fmod((60 * ((rgb[0] - rgb[1]) / diff) + 240), 360);

    if (max_val == 0)
        hsv[1] = 0;

    else
        hsv[1] = (diff / max_val) * 100;

    hsv[2] = max_val * 100;
}

double hsv_to_rgb_sub(double h, double s, double v, double n)
{
    double k = fmod((n + h / 60), 6);
    return v - v * s * max(0, min(k, min(4 - k, 1)));
}

void hsv_to_rgb(double h, double s, double v, double *rgb)
{
    s /= 100;
    v /= 100;

    rgb[0] = hsv_to_rgb_sub(h, s, v, 5);
    rgb[1] = hsv_to_rgb_sub(h, s, v, 3);
    rgb[2] = hsv_to_rgb_sub(h, s, v, 1);
}