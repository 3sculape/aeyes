#include "../utils/lib_sdl.h"
#include "effects.h"
#include "basic.h"
#include "utility.h"
#include <math.h>

void swirl(SDL_Surface *surface, double factor)
{
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    int w = surface->w;
    int h = surface->h;
    int midx = w / 2;
    int midy = h / 2;

    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            int dx = i - midx;
            int dy = j - midy;
            double theta = atan2(dy, dx);
            double radius = sqrt(dx * dx + dy * dy);
            double newx = midx +
                (radius * cos(theta + (factor * radius * M_PI) / 180));
            double newy = midy +
                (radius * sin(theta + (factor * radius * M_PI) / 180));
            if(newx > 0 && newx < w && newy > 0 && newy < h)
            {
                Uint32 pixel = get_pixel(surface, newx, newy);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(surface2, r, g, b, a, i, j);
            }
        }
    }


    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}

void twist(SDL_Surface *surface, double factor)
{
    if(SDL_LockSurface(surface) != 0)
        return;
    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    int w = surface->w;
    int h = surface->h;

    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            float dx = (factor * sin(2 * M_PI * j / 64));
            float dy = (factor * sin(2 * M_PI * i / 64));

            float newx = i + dx;
            float newy = j + dy;

            if(newx > 0 && newx < w && newy > 0 && newy < h)
            {
                Uint32 pixel = get_pixel(surface, newx, newy);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                set_pixel(surface2, r, g, b, a, i, j);
            }
        }
    }
    SDL_Surface *surface3 = image_crop(surface2, (size_t)factor + 1,
            (size_t)factor + 1, surface2->w - 2 * (size_t)factor - 1,
            surface2->h - 2 * (size_t)factor - 1);

    SDL_Surface * surface4 = scale_strech(surface3, surface->w, surface->h);

    copy_surface(surface4, surface);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surface4);
    SDL_UnlockSurface(surface);
}
