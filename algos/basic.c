#include "basic.h"

void grayscale(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in grayscale");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            Uint8 total = 0.21 * r + 0.72 * g + 0.07 * b;
            set_pixel(surface, total, total, total, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void binarization(SDL_Surface *surface, char threshold)
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
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            if ((r + g + b) / 3 > threshold)
            {
                r = 255;
                g = 255;
                b = 255;
            }

            else
            {
                r = 0;
                g = 0;
                b = 0;
            }

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void negative(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in negative");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

void clipping(SDL_Surface *surface)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in clipping");
        return;
    }

    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            if (r == 255 && g == 255 && b == 255)
            {
                g = 0;
                b = 0;
            }

            else if (r == 0 && g == 0 && b == 0)
            {
                b = 255;
            }

            set_pixel(surface, r, g, b, a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
}

SDL_Texture* image_crop(SDL_Texture* original, SDL_Renderer* renderer, 
        size_t x, size_t y, size_t w, size_t h)
{
    SDL_Rect src;

    src.x = x;
    src.y = y;
    src.w = w;
    src.h = h;

    SDL_Texture* cropped = create_texture(renderer, w, h);

    if(SDL_SetRenderTarget(renderer, cropped))
        warnx("SetRenderTarget fail");

    if (SDL_RenderCopy(renderer, original, src, NULL))
        warnx("RenderCopy fail");

    return cropped;
}

SDL_Texture* resize(SDL_Surface* original,SDL_Renderer* render, double factor)
{
    return surface_to_texture(rotozoomSurface(original, 0, factor, 1),render);
}
