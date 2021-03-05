#include "convert.h"

void rgb_to_hsv(Uint8 r, Uint8 g, Uint8 b, double *hsv)
{
    double rgb[3];
    double max;
    double min;
    double diff;

    rgb[0] = r / 255.0;
    rgb[1] = g / 255.0;
    rgb[2] = b / 255.0;

    max = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
    min = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
    diff = max - min;

    if (max == min)
        hsv[0] = 0;

    else if (max == rgb[0])
        hsv[0] = fmod((60 * ((rgb[1] - rgb[2]) / diff) + 360), 360);

    else if (max == rgb[1])
        hsv[0] = fmod((60 * ((rgb[2] - rgb[0]) / diff) + 120), 360);

    else if (max == rgb[2])
        hsv[0] = fmod((60 * ((rgb[0] - rgb[1]) / diff) + 240), 360);

    if (max == 0)
        hsv[1] = 0;

    else
        hsv[1] = (diff / max) * 100;

    hsv[2] = max * 100;
}