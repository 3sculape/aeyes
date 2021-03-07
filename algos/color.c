#include "color.h"

void brightest_pixel(SDL_Surface *surface, int *coordinates)
{
    Uint8 max = 0;
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 average = (r + g + b) / 3;

            if (average > max)
            {
                max = average;
                coordinates[0] = i;
                coordinates[1] = j;
            }
        }
    }
}