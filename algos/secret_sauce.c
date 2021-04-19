#include "secret_sauce.h"

void deep_fry(SDL_Surface *surface, Uint8 *reference)
{
    if (SDL_LockSurface(surface) != 0)
    {
        warnx("LockSurface fail in white_balance");
        return;
    }

    gsl_matrix *m = gsl_matrix_calloc(3, 3);
    gsl_vector *c = gsl_vector_calloc(3);
    gsl_vector *res = gsl_vector_calloc(3);

    gsl_matrix_set(m, 0, 0, 255 / reference[0]);
    gsl_matrix_set(m, 1, 1, 255 / reference[1]);
    gsl_matrix_set(m, 2, 2, 255 / reference[2]);
    for (int i = 0; i < surface -> w; i++)
    {
        for (int j = 0; j < surface -> h; j++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface -> format, &r, &g, &b, &a);
            c -> data[0] = r;
            c -> data[1] = g;
            c -> data[2] = b;

            gsl_blas_dgemv(CblasNoTrans, 1, m, c, 0, res);

            set_pixel(surface, res -> data[0], res -> data[1], res -> data[2],
             a, i, j);
        }
    }

    SDL_UnlockSurface(surface);
    gsl_matrix_free(m);
    gsl_vector_free(c);
    gsl_vector_free(res);
}

SDL_Surface* fever_dream(SDL_Surface *surface, double force)
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

void conv(SDL_Surface *surface,SDL_Surface* copy
        ,int x, int y, gsl_matrix* ker)
{
    Uint8 nr, ng, nb, a;
    nr = 0;
    ng = 0;
    nb = 0;
    for (int offsetx = -1; offsetx < 2; offsetx++)
    {
        for (int offsety = -1; offsety < 2; offsety++)
        {
            Uint8 r, g, b;
            double kernelvalue = gsl_matrix_get(ker, 1 + offsetx, 1 + offsety);
            Uint32 pixel = get_pixel(surface, x + offsetx, y + offsety);
            if (pixel == (Uint32) -1)
            {
                set_pixel(copy, 255, 0, 0, a, x, y);
                return;
            }
            else
            {
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                double tmpr = kernelvalue * (double)r;
                double tmpg = kernelvalue * (double)g;
                double tmpb = kernelvalue * (double)b;
                nr += (Uint8)tmpr;
                ng += (Uint8)tmpg;
                nb += (Uint8)tmpb;
            }
        }
    }
    set_pixel(copy, nr, ng, nb, a, x, y);
}

void mercury(SDL_Surface *surface)
{
    SDL_Surface* copy = create_surface(surface->w, surface->h);
    copy_surface(surface, copy);
    gaussian_blur(copy, 5);

    gsl_matrix* Gx = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gx, 0, 0, -1);
    gsl_matrix_set(Gx, 0, 1, 0);
    gsl_matrix_set(Gx, 0, 2, 1);
    gsl_matrix_set(Gx, 1, 0, -2);
    gsl_matrix_set(Gx, 1, 1, 0);
    gsl_matrix_set(Gx, 1, 2, 2);
    gsl_matrix_set(Gx, 2, 0, -1);
    gsl_matrix_set(Gx, 2, 1, 0);
    gsl_matrix_set(Gx, 2, 2, 1);
    SDL_Surface* copyGx = create_surface(surface->w, surface->h);
    for(int i = 0; i < copy->w; i++)
    {
        for (int j = 0; j < copy -> h; j++)
        {
            conv(copy, copyGx, i, j, Gx);
        }
    }
    copy_surface(copyGx, surface);
    gsl_matrix_free(Gx);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(copyGx);
}

void convolve_color(SDL_Surface *surface,SDL_Surface* copy
        ,int x, int y, gsl_matrix* ker)
{
    // Variable definitions
    double nr;
    double ng;
    double nb;
    Uint8 a = 0;
    nr = 0;
    ng = 0;
    nb = 0;

    // Going through the kernel
    for (int offsetx = -1; offsetx < 2; offsetx++)
    {
        for (int offsety = -1; offsety < 2; offsety++)
        {
            Uint8 r, g, b;
            double kernelvalue = gsl_matrix_get(ker, 1 + offsetx, 1 + offsety);

            Uint32 pixel = get_pixel(surface, x + offsety, y + offsetx);
            if (pixel == (Uint32) -1)
            {
                //warnx("fail pixel");
                set_pixel(copy, 0, 0, 0, a, x, y);
                return;
            }
            else
            {
                // Simple sum in function of neighbors with kernel
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                double tmpr = kernelvalue * (double)r;
                double tmpg = kernelvalue * (double)g;
                double tmpb = kernelvalue * (double)b;
                nr += tmpr;
                ng += tmpg;
                nb += tmpb;
            }
        }
    }
    nr = fabs(nr);
    ng = fabs(ng);
    nb = fabs(nb);
    set_pixel(copy, (Uint8)nr, (Uint8)ng, (Uint8)nb, a, x, y);
}

SDL_Surface* neon(SDL_Surface *surface)
{
    // Defining temporary surfaces to work on
    SDL_Surface* copy = create_surface(surface->w, surface->h);
    copy_surface(surface, copy);
    // Preprocessing (noise reduction)
    gaussian_blur(copy, 5);

    // Definition of Gradient kernels
    gsl_matrix* Gx = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gx, 0, 0, -1);
    gsl_matrix_set(Gx, 0, 1, 0);
    gsl_matrix_set(Gx, 0, 2, 1);
    gsl_matrix_set(Gx, 1, 0, -2);
    gsl_matrix_set(Gx, 1, 1, 0);
    gsl_matrix_set(Gx, 1, 2, 2);
    gsl_matrix_set(Gx, 2, 0, -1);
    gsl_matrix_set(Gx, 2, 1, 0);
    gsl_matrix_set(Gx, 2, 2, 1);
    SDL_Surface* copyGx = create_surface(surface->w, surface->h);
    // Convolution
    for(int i = 0; i < copy->w; i++)
    {
        for (int j = 0; j < copy -> h; j++)
        {
            convolve_color(copy, copyGx, i, j, Gx);
        }
    }
    gsl_matrix* Gy = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(Gy, 0, 0, 1);
    gsl_matrix_set(Gy, 0, 1, 2);
    gsl_matrix_set(Gy, 0, 2, 1);
    gsl_matrix_set(Gy, 1, 0, 0);
    gsl_matrix_set(Gy, 1, 1, 0);
    gsl_matrix_set(Gy, 1, 2, 0);
    gsl_matrix_set(Gy, 2, 0, -1);
    gsl_matrix_set(Gy, 2, 1, -2);
    gsl_matrix_set(Gy, 2, 2, -1);
    SDL_Surface* copyGy = create_surface(surface->w, surface->h);
    for(int i = 0; i < copy->w; i++)
    {
        for (int j = 0; j < copy -> h; j++)
        {
            convolve_color(copy, copyGy, i, j, Gy);
        }
    }
    SDL_Surface* hypot = create_surface(surface->w, surface->h);
    // Computing sqrt(Gx² + Gy²) and theta(y,x) = atan(y/x)
    for(int i = 1; i < hypot -> w - 1; i++)
    {
        for (int j = 1; j < hypot -> h - 1; j++)
        {
            Uint8 a; //r, g, b;
            Uint8 xr, xg, xb;
            Uint8 yr, yg, yb;
            Uint32 pixelx = get_pixel(copyGx, i, j);
            Uint32 pixely = get_pixel(copyGy, i, j);

            SDL_GetRGBA(pixelx, copyGx -> format, &xr, &xg, &xb, &a);
            SDL_GetRGB(pixely, copyGy -> format, &yr, &yg, &yb);
            double nr = gsl_hypot((double)xr, (double)yr);
            double ng = gsl_hypot((double)xg, (double)yg);
            double nb = gsl_hypot((double)xb, (double)yb);
            set_pixel(hypot, (Uint8)nr, (Uint8)ng, (Uint8)nb, a, i, j);
        }
    }
    SDL_FreeSurface(copyGx);
    SDL_FreeSurface(copyGy);
    SDL_FreeSurface(copy);
    gsl_matrix_free(Gx);
    gsl_matrix_free(Gy);
    return hypot;
}
void get_pixel_hist(SDL_Surface *surface, int *rhisto, int *bhisto,
        int *ghisto, int posx, int posy, int x)
{
    for(int j = posy - (x - 1) / 2; j <= posy + (x - 1) / 2; j++)
    {
        for(int i = posx - (x - 1) / 2; i <= posx + (x - 1) / 2; i++)
        {
            Uint32 pixel;
            if(j < 0)
            {
                if(i < 0)
                {
                    pixel = get_pixel(surface, i + (x - 1) / 2,
                            j + (x - 1) / 2);
                }
                else if(i >= surface->w)
                {
                    pixel = get_pixel(surface, i - (x - 1) / 2,
                            j + (x - 1) / 2);
                }
                else
                {
                    pixel = get_pixel(surface, i, j + (x - 1) / 2);
                }
            }

            else if(j >= surface->h)
            {
                if(i < 0)
                {
                    pixel = get_pixel(surface, i + (x - 1) / 2,
                            j - (x - 1) / 2);
                }
                else if(i >= surface->w)
                {
                    pixel = get_pixel(surface, i - (x - 1) / 2,
                            j - (x - 1) / 2);
                }
                else
                {
                    pixel = get_pixel(surface, i, j - (x - 1) / 2);
                }
            }

            else if(i < 0)
            {
                pixel = get_pixel(surface, i + (x - 1) / 2, j);
            }
            else if(i >= surface->w)
            {
                pixel = get_pixel(surface, i - (x - 1) / 2, j);
            }
            else
            {
                pixel = get_pixel(surface, i, j);
            }

            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            rhisto[r] += 1;
            ghisto[g] += 1;
            bhisto[b] += 1;
        }
    }
}

void get_median_in_hist(int *histo, Uint8 *res, int x)
{
    int count = 0;
    Uint8 i;
    for(i = 0; count < (x * x - 1) / 2; i++)
    {
        count += histo[i];
    }
    *res = i - 1;
}

void get_nex_col(SDL_Surface *surface, int *rhisto, int *ghisto, int *bhisto,
        int posx, int posy, int x)
{
    for(int j = posy - (x - 1) / 2; j <= posy + (x - 1) / 2; j++)
    {
        Uint32 pixel = get_pixel(surface, posx - (x - 1) / 2, j);
        Uint32 pixel2 = get_pixel(surface, posx + (x - 1) / 2 + 1, j);
        Uint8 r, g, b, r2, g2, b2;
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        SDL_GetRGB(pixel2, surface->format, &r2, &g2, &b2);
        rhisto[r] -= 1;
        ghisto[g] -= 1;
        bhisto[b] -= 1;
        rhisto[r2] += 1;
        ghisto[g2] += 1;
        bhisto[b2] += 1;

    }
}

void glitch(SDL_Surface *surface, int x)
{
    if(x % 2 == 0)
        x += 1;
    if(SDL_LockSurface(surface) != 0)
        return;

    SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0,
            surface->w, surface->h, 32, surface->format->format);
    int *rhisto = malloc(256 * sizeof(int));
    int *ghisto = malloc(256 * sizeof(int));
    int *bhisto = malloc(256 * sizeof(int));

    for(int j = 0; j < surface->h; j++)
    {
        memset(rhisto, 0, 256 * sizeof(int));
        memset(ghisto, 0, 256 * sizeof(int));
        memset(bhisto, 0, 256 * sizeof(int));
        get_pixel_hist(surface, rhisto, ghisto, bhisto, 0, j, x);
        for(int i = 0; i < surface->w; i++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            get_median_in_hist(rhisto, &r, x);
            get_median_in_hist(ghisto, &g, x);
            get_median_in_hist(bhisto, &b, x);
            set_pixel(surface2, r, g, b, a, i, j);
            get_nex_col(surface, rhisto, ghisto, bhisto, i, j, x);
        }
    }

    free(rhisto);
    free(ghisto);
    free(bhisto);
    copy_surface(surface2, surface);
    SDL_FreeSurface(surface2);
    SDL_UnlockSurface(surface);
}