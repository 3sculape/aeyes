#include "trailing.h"

void global_trailing_v(SDL_Surface *surface, int start_pixel, int inverse)
{
    int image_h = surface->h;
    int image_w = surface->w;
    Uint8 r, g, b, a;
    Uint32 pixel;
    int kernel_size;

    if (inverse == 0)
    {
        kernel_size = image_w - start_pixel;

        for (int j = 0; j < image_h; j++)
        {
            pixel = get_pixel(surface, start_pixel, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            for (int i = 1; i < kernel_size; i++)
            {
                set_pixel(surface, r, g, b, a, start_pixel + i, j);
            }
        }
    }

    if (inverse == 1)
    {
        kernel_size = start_pixel;

        for (int j = 0; j < image_h; j++)
        {
            pixel = get_pixel(surface, start_pixel, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            for (int i = start_pixel-1; i >= 0; i--)
            {
                set_pixel(surface, r, g, b, a, i, j);
            }
        }
    }
}





void global_trailing_h(SDL_Surface *surface, int start_pixel, int inverse)
{
    int image_h = surface->h;
    int image_w = surface->w;
    Uint8 r, g, b, a;
    Uint32 pixel;
    int kernel_size;

    if (inverse == 0)
    {
        kernel_size = image_h - start_pixel;

        for (int i = 0; i < image_w; i++)
        {
            pixel = get_pixel(surface, i, start_pixel);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            for (int j = 1; j < kernel_size; j++)
            {
                set_pixel(surface, r, g, b, a, i, j+start_pixel);
            }
        }
    }

    if (inverse == 1)
    {
        kernel_size = start_pixel;

        for (int i = 0; i < image_w; i++)
        {
            pixel = get_pixel(surface, i, start_pixel);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);

            for (int j = start_pixel-1; j >= 0; j--)
            {
                set_pixel(surface, r, g, b, a, i, j);
            }
            
        }
    }
}