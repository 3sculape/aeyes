#include "../utils/lib_sdl.h"
#include <stdio.h>
#include <err.h>
#include "unsharp_masking.h"
#include "blurs.h"

SDL_Surface* sharpen(SDL_Surface *surface, double force, SDL_Renderer *renderer)
{
    if (surface == NULL)
        goto error;
    SDL_Surface* res = create_surface(surface->w, surface->h);
    if (res == NULL)
        goto error;
    SDL_Rect tmp;
    SDL_Texture* original = create_texture(renderer, surface->w, surface->h);
    SDL_SetRenderTarget(renderer, original);
    SDL_Texture* s = surface_to_texture(surface, renderer);
    SDL_RenderCopy(renderer, s, &tmp, NULL);
    box_blur(surface);
    saveJPG("blur.JPG", surface);
    SDL_Surface* org = texture_to_surface(original, renderer);
    for (int i = 0; i < surface->w; i++)
    {
        for (int j = 0; j < surface->h; j++)
        {
            Uint8 or, og, ob, a;
            or = 0;
            og = 0;
            ob = 0;
            a = 0;
            Uint8 br, bg, bb;
            br = 0;
            bg = 0;
            bb = 0;

            Uint32 pixelorg = get_pixel(org, i, j);
            Uint32 pixelblr = get_pixel(surface, i, j);

            SDL_GetRGBA(pixelorg, org->format, &or, &og, &ob, &a);
            SDL_GetRGB(pixelblr, surface->format, &br, &bg, &bb);
            Uint8 r, g, b;
            r = or + ((or - br) * force);
            g = og + ((og - bg) * force);
            b = ob + ((ob - bb) * force);
            set_pixel(res, r, g, b, a, i, j);
        }
    }
    SDL_DestroyTexture(original);
    SDL_DestroyTexture(s);
    SDL_FreeSurface(org);
    return res;
error: warnx("unsharp mask function fail");
       return NULL;
}

SDL_Surface* sharpen2(SDL_Surface *surface, double force)
{
    if (surface == NULL)
        goto error;
    SDL_Surface* res = create_surface(surface->w, surface->h);
    if (res == NULL)
        goto error;
    gsl_matrix* kernel = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(kernel, 0, 0, 0);
    gsl_matrix_set(kernel, 0, 1, -1 * force);
    gsl_matrix_set(kernel, 0, 2, 0);
    gsl_matrix_set(kernel, 1, 0, -1 * force);
    gsl_matrix_set(kernel, 1, 1, 5 * force);
    gsl_matrix_set(kernel, 1, 2, -1 * force);
    gsl_matrix_set(kernel, 2, 0, 0);
    gsl_matrix_set(kernel, 2, 1, -1 * force);
    gsl_matrix_set(kernel, 2, 2, 0);
    for (int i = 0; i < surface->w; i++)
    {
        for (int j = 0; j < surface->h; j++)
        {
            Uint8 r, g, b, a;
            Uint8 nr, ng, nb;
            r = 0;
            g = 0;
            b = 0;
            a = 0;
            nr = 0;
            ng = 0;
            nb = 0;
            for (int kj = -1; kj < 2; kj++)
            {
                for (int ki = -1; ki < 2; ki++)
                {
                    Uint32 pixel = get_pixel(surface, i, j);
                    /*if (pixel == -1)
                        goto error;*/
                    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                    nr += (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * r);
                    ng += (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * g);
                    nb += (Uint8)(gsl_matrix_get(kernel, kj + 1, ki + 1) * b);
                }
            }
            set_pixel(res, nr, ng, nb, a, i, j);
        }
    }
    return res;

error: warnx("unsharp mask function fail");
       return NULL;
}
