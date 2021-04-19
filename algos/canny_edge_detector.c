#include "canny_edge_detector.h"

void convolve_around_pixel(SDL_Surface *surface,SDL_Surface* copy
        ,int x, int y, gsl_matrix* ker)
{
    // Variable definitions
    double nr;
    Uint8 a = 0;
    nr = 0;

    // Going through the kernel
    for (int offsetx = -1; offsetx < 2; offsetx++)
    {
        for (int offsety = -1; offsety < 2; offsety++)
        {
            Uint8 r, g, b;
            double kernelvalue = gsl_matrix_get(ker, 1 +offsetx,1+offsety);

            Uint32 pixel = get_pixel(surface,x + offsety,y +offsetx);
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
                nr += tmpr;
            }
        }
    }
    nr = fabs(nr);
    set_pixel(copy, (Uint8)nr, (Uint8)nr, (Uint8)nr, a, x, y);
}

void find_strongest(SDL_Surface* surface, int posx, int posy)
{
    Uint8 checkr, checkg, checkb;
    Uint32 check = get_pixel(surface, posx, posy);
    SDL_GetRGB(check, surface->format, &checkr, &checkg, &checkb);
    if (checkr == 0)
        return;
    for(int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; ++j)
        {
            Uint8 r, g, b, a;
            int x = posx + j;
            int y = posy + i;
            Uint32 pixel = get_pixel(surface, x, y);
            if (pixel == (Uint32)-1)
                continue;
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            if (r == 255)
            {
                set_pixel(surface, 255, 255, 255, a, posx, posy);
                return;
            }
        }
    }
    set_pixel(surface, 0, 0, 0, 1, posx, posy);
}

SDL_Surface* canny_fnc(SDL_Surface *surface)
{
    // Defining temporary surfaces to work on
    SDL_Surface* canny = create_surface(surface->w, surface->h);
    SDL_Surface* copy = create_surface(surface->w, surface->h);
    copy_surface(surface, copy);
    // Preprocessing (noise reduction)
    grayscale(copy);
    gaussian_blur(copy, 7);
    savePNG("blur.PNG", copy);

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
            convolve_around_pixel(copy, copyGx, i, j, Gx);
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
            convolve_around_pixel(copy, copyGy, i, j, Gy);
        }
    }

    savePNG("gx.PNG", copyGx);
    savePNG("gy.PNG", copyGy);

    SDL_Surface* hypot = create_surface(surface->w, surface->h);
    gsl_matrix* theta = gsl_matrix_calloc(surface->w - 1, surface->h-1);
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
            double tmpx = (double)xr;
            double tmpy = (double)yr;
            // if atan y / x doesn't work, go for atan2(y, x);
            gsl_matrix_set(theta, i - 1, j - 1, atan2(tmpy , tmpx));
        }
    }
    savePNG("hypot.PNG", hypot);
    SDL_Surface* nonmax = create_surface(surface->w, surface->h);
    Uint8 maxp = 0;
    for (int i = 1; i < hypot->w - 1; i++)
    {
        for (int j = 1; j < hypot->h - 1; j++)
        {
            Uint8 qr, qg, qb;
            Uint8 rr, rg, rb;
            Uint32 q = 0;
            Uint32 r = 0;
            double intensity = gsl_matrix_get(theta, i-1, j-1);
            if (intensity < 22.5 || (intensity >=157.5 && intensity <= 180))
            {
                if (intensity >= 0)
                {
                    q = get_pixel(hypot, i, j + 1);
                    r = get_pixel(hypot, i, j - 1);
                }
            }
            else if (intensity >= 22.5 && intensity < 67.5)
            {
                q = get_pixel(hypot, i + 1, j - 1);
                r = get_pixel(hypot, i - 1, j + 1);
            }
            else if (intensity >= 67.5  && intensity < 112.5)
            {
                q = get_pixel(hypot, i + 1, j);
                r = get_pixel(hypot, i - 1, j);
            }
            else if (intensity >= 112.5 && intensity < 157.5)
            {
                q = get_pixel(hypot, i - 1, j - 1);
                r = get_pixel(hypot, i + 1, j + 1);
            }
            else
            {
                //printf("Intensity fail\n");
                continue;
            }
            Uint32 pixel = get_pixel(hypot, i, j);
            Uint8 re, g, b;
            SDL_GetRGB(pixel, hypot -> format, &re, &g, &b);
            SDL_GetRGB(q, hypot->format, &qr, &qg, &qb);
            SDL_GetRGB(r, hypot->format, &rr, &rg, &rb);
            if (re >= qr && re >= rr)
            {
                if (re > maxp)
                    maxp = re;
                //printf("setting max intensity\n");
                set_pixel(nonmax, re, g, b, 1, i, j);
            }
            else
            {
                //printf("setting black pixel\n");
                set_pixel(nonmax, 0, 0, 0, 1, i, j);
            }
        }
    }
    savePNG("nonmax.PNG", nonmax);

    const double maxratio = 0.15;
    const double lowratio = 0.05;

    /*Uint8 highthreshold = (Uint8)((double)maxp * maxratio);
    Uint8 lowthreshold = (Uint8)((double)maxp * lowratio);*/
    // Hysterisis
    SDL_Surface* dualthresh = create_surface(surface->w, surface->h);

    for(int i = 0; i < nonmax->w; i++)
    {
        for(int j = 0; j < nonmax->h; j++)
        {
            Uint8 r, g, b;
            Uint32 pixel = get_pixel(nonmax, i, j);
            SDL_GetRGB(pixel, nonmax->format, &r, &g, &b);
            if ((double)r/255.0 > maxratio)
                set_pixel(dualthresh, 255, 255,
                          255, 1, i, j);
            else if ((double)r/255.0 < lowratio)
                set_pixel(dualthresh, 0, 0, 0, 1, i, j);
            else
                set_pixel(dualthresh, r, g, b, 1, i, j);
        }
    }

    savePNG("dualthresh.PNG", dualthresh);
    for (int i = 0; i < dualthresh->w; ++i) {
        for (int j = 0; j < dualthresh->h; ++j) {
            find_strongest(dualthresh, i, j);
        }
    }
    copy_surface(dualthresh, canny);
    // Memory cleanup
    gsl_matrix_free(Gx);
    gsl_matrix_free(Gy);
    gsl_matrix_free(theta);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(copyGx);
    SDL_FreeSurface(copyGy);
    SDL_FreeSurface(hypot);
    SDL_FreeSurface(dualthresh);
    SDL_FreeSurface(nonmax);
    return canny;
}
